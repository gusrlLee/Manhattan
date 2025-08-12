import mitsuba as mi
import drjit as dr 
mi.set_variant('llvm_ad_rgb')

class MyPathTracer(mi.SamplingIntegrator):
    def __init__(self, props=mi.Properties()):
        super().__init__(props)
        self.max_depth = props.get("max_depth", 10)
        self.rr_depth = props.get("rr_depth", 5)

    @dr.syntax
    def sample(self, 
               scene: mi.Scene, 
               sampler: mi.Sampler, 
               ray: mi.RayDifferential3f, 
               medium: mi.Medium = None, 
               active: bool = True):

        bsdf_ctx = mi.BSDFContext()
        wi = mi.Ray3f(ray)
        depth = mi.UInt32(0)
        thp = mi.Spectrum(1.0)
        L = mi.Spectrum(0.0)

        si: mi.SurfaceInteraction3f = scene.ray_intersect(wi, active)
        active = si.is_valid()
        if active:
            return (mi.Spectrun(1.0), [], [])
        else:
            return (mi.Spectrun(0.0), [], [])




if __name__ == "__main__":
    mi.register_integrator("PathTracer", lambda props: MyPathTracer(props))
    myPath = mi.load_dict({
        'type' : 'PathTracer',
        'max_depth' : 10,
        'rr_depth' : 5
    })

    scene = mi.load_file("./veach-mis/scene.xml")
    image = mi.render(scene, integrator=myPath)
    mi.util.write_bitmap("output.png", image)





            





