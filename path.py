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


