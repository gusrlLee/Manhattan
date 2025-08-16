import mitsuba as mi
import drjit as dr
if 'cuda_ad_rgb' in mi.variants():
    mi.set_variant('cuda_ad_rgb')
else:
    mi.set_variant('llvm_ad_rgb')

class Pt(mi.SamplingIntegrator):
    def __init__(self, props=mi.Properties()):
        super().__init__(props)
        self.max_depth = props.get("max_depth", 10)
        self.rr_depth = props.get("rr_depth", 5)    
    
    @dr.syntax
    def sample(self, scene: mi.Scene, sampler: mi.Sampler, ray: mi.RayDifferential3f, medium: mi.Medium = None, active: bool = True):
        bsdf_ctx: mi.BSDFContext = mi.BSDFContext()
        r: mi.Ray3f = mi.Ray3f(ray)
        depth = mi.UInt32(0)
        throughput: mi.Spectrum = mi.Spectrum(1.0)
        L: mi.Spectrum = mi.Spectrum(0.0)

        prev_si = dr.zeros(mi.SurfaceInteraction3f)

        while (active):
            si: mi.SurfaceInteraction3f = scene.ray_intersect(r) # Traversal ray 
            ds = mi.DirectionSample3f(scene=scene, si=si, ref=prev_si) # calculate ray interaction about surface
            Le = throughput * ds.emitter.eval(si=si) # Le 

            next_traversal = (depth + 1 < self.max_depth) & si.is_valid()
            bsdf: mi.BSDF = si.bsdf(r)
            bsdf_sample, bsdf_val = bsdf.sample(bsdf_ctx, si, sampler.next_1d(), sampler.next_2d(), next_traversal)

            r = si.spawn_ray(si.to_world(bsdf_sample.wo))
            if (depth == 5):
                L = (L + Le)
            throughput *= bsdf_val

            prev_si = dr.detach(si, True)
            active = next_traversal
            depth += 1

        return (L, (depth != 0), [])

class Pt_mis(mi.SamplingIntegrator):
    def __init__(self, props=mi.Properties()):
        super().__init__(props)
        self.max_depth = props.get("max_depth", 10)

    @dr.syntax
    def sample(self, scene: mi.Scene, sampler: mi.Sampler, ray: mi.RayDifferential3f, medium: mi.Medium = None, active: bool = True):
        return 0