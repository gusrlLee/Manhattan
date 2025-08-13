import drjit as dr
import mitsuba as mi
import time

mi.set_variant('cuda_ad_rgb')

class Foo:
    def __init__(self, var_init=32, rng_size=102400):
        # 멤버 변수 초기화
        self.var = mi.Float(var_init)
        self.rng = mi.PCG32(size=rng_size)

    @dr.syntax
    def step(self):
        count = mi.UInt(0)

        while count < 2139992392:
            self.var += self.rng.next_float32()
            count += 1

        # return 하지 않아도 멤버 변수에 반영됨
        return self.var, self.rng

    def increment(self, amount=1):
        self.var += amount

class PathTracer(mi.SamplingIntegrator):
    def __init__(self, props=mi.Properties()):
        super().__init__(props)
        self.max_depth = props.get("max_depth", 10)
        self.rr_depth = props.get("rr_depth", 5)

    @dr.syntax
    def sample(self, 
               scn: mi.Scene,
               sampler: mi.Sampler,
               ray: mi.Ray3f,
               medium: mi.Medium = None,
               active: bool = True
               ):
        
        bsdf_ctx = mi.BSDFContext()
        wi = mi.Ray3f(ray)
        depth = mi.UInt32(0)
        thp = mi.Spectrum(1.0)
        L = mi.Spectrum(0.0)
        prev_si = dr.zeros(mi.SurfaceInteraction3f)

        while (active):
            si: mi.SurfaceInteraction3f = scn.ray_intersect(ray)
            ds = mi.DirectionSample3f(scn, si=si, ref=prev_si)
            Le = thp * ds.emitter.eval(si)
            active_next = (depth + 1 < self.max_depth) & si.is_valid()
            bsdf: mi.BSDF = si.bsdf(wi)
            bsdf_sample, bsdf_val = bsdf.sample(bsdf_ctx, si, sampler.next_1d(), sampler.next_2d(), active_next)
            wi = si.spawn_ray(si.to_world(bsdf_sample.wo))
            L = (L + Le)
            thp *= bsdf_val
            prev_si = dr.detach(si, True)
            rr_prop = dr.maximum(thp.x, dr.maximum(thp.y, thp.z))
            rr_prop[depth < self.rr_depth] = 1.
            thp *= dr.rcp(rr_prop)
            active_next &= (sampler.next_1d() < rr_prop)
            active = active_next
            depth += 1

        return (L, (depth != 0), [])
    
if __name__ == "__main__":
    mi.register_integrator("mypath", lambda props: PathTracer(props))
    my_path = mi.load_dict({
        'type' : 'mypath',
        'max_depth' : 10,
        'rr_depth' : 5
    })

    scene = mi.load_file("./scenes/cornell-box/scene.xml")
    start = time.perf_counter()
    image = mi.render(scene, spp=1024, integrator=my_path)
    end = time.perf_counter()
    mi.util.write_bitmap("our.png", image)
    print(f"Rendering time = {(end - start) / 1000} ms")