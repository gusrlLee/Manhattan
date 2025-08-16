import mitsuba as mi 
import drjit as dr 

if 'cuda_ad_rgb' in mi.variants():
    mi.set_variant('cuda_ad_rgb')
else:
    mi.set_variant('llvm_ad_rgb')

class pt_mis(mi.SamplingIntegrator):
    def __init__(self, props=mi.Properties()):
        super().__init__(props)
        self.max_depth = props.get("max_depth", 10)
        self.rr_depth = props.get("rr_depth", 5)

    def mis_weight(self, pdf_a, pdf_b):
        """
        Compute the Multiple Importance Sampling (MIS) weight given the densities
        of two sampling strategies accroding to the power heuristic.
        """
        a2 = dr.sqr(pdf_a)
        return dr.detach(dr.select(pdf_a > 0, a2 / dr.fma(pdf_b, pdf_b, a2), 0), True)

    @dr.syntax 
    def sample(self, 
               scn: mi.Scene, 
               sampler: mi.Sampler,
               ray: mi.RayDifferential3f, 
               medium: mi.Medium = None, 
               active: bool = True):
        
        # Configure loop state
        bsdf_ctx = mi.BSDFContext()
        wi = mi.Ray3f(ray)
        depth = mi.UInt32(0)
        throughput = mi.Spectrum(1.0)
        radiance = mi.Spectrum(0.0)

        eta = mi.Float(1.0)
        mis_em = mi.Float(1.0)

        prev_si = dr.zeros(mi.SurfaceInteraction3f)

        while (active):
            si: mi.SurfaceInteraction3f = scn.ray_intersect(
                ray=wi, 
                ray_flags=mi.RayFlags.All, 
                coherent=(depth == 0), 
                reorder=False, 
                reorder_hint=0, 
                reorder_hint_bits=0, 
                active=active)

            ds = mi.DirectionSample3f(scene=scn, si=si, ref=prev_si)
            Le = throughput * mis_em * ds.emitter.eval(si)

            active_next = (depth + 1 < self.max_depth) & si.is_valid()

            bsdf: mi.BSDF = si.bsdf(wi)
            active_em = active_next & mi.has_flag(bsdf.flags(), mi.BSDFFlags.Smooth)

            ds, em_weight = scn.sample_emitter_direction(si, sampler.next_2d(), True, active_em)
            active_em &= (ds.pdf != 0.0)

            wo = si.to_local(ds.d)
            bsdf_value_em, bsdf_pdf_em = bsdf.eval_pdf(bsdf_ctx, si, wo, active_em)
            mis_direct = dr.select(ds.delta, 1, self.mis_weight(ds.pdf, bsdf_pdf_em))
            Lr_dir = throughput * mis_direct * bsdf_value_em * em_weight

            bsdf_sample, bsdf_val = bsdf.sample(bsdf_ctx, si, sampler.next_1d(), sampler.next_2d(), active_next)
            bsdf_sample_delta = mi.has_flag(bsdf_sample.sampled_type, mi.BSDFFlags.Delta)

            eta *= bsdf_sample.eta
            throughput *= bsdf_val
            radiance = radiance + Le + Lr_dir

            prev_si = dr.detach(si, True)

            thp_max = dr.maximum(throughput.x, dr.maximum(throughput.y, throughput.z))
            active_next &= (thp_max != 0)

            rr_prob = dr.minimum(thp_max * eta ** 2, .95)

            rr_active = depth >= self.rr_depth
            throughput[rr_active] *= dr.rcp(rr_prob)
            rr_continue = sampler.next_1d() < rr_prob
            active_next &= ~rr_active | rr_continue

            ray_next = si.spawn_ray(si.to_world(bsdf_sample.wo))
            pi_next = scn.ray_intersect_preliminary(ray_next, active=active_next)

            si_next = pi_next.compute_surface_interaction(ray_next)
            ds = mi.DirectionSample3f(scn, si=si_next, ref=si)

            pdf_em = scn.pdf_emitter_direction(ref=si, ds=ds, active=~bsdf_sample_delta)
            mis_em = self.mis_weight(bsdf_sample.pdf, pdf_em)

            pi = pi_next
            wi = ray_next
            active = active_next 
            depth += 1
        
        return (radiance, (depth != 0), [])