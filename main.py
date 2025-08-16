import drjit as dr
import mitsuba as mi
import time
from pt import Pt 


if 'cuda_ad_rgb' in mi.variants():
    mi.set_variant('cuda_ad_rgb')
else:
    mi.set_variant('llvm_ad_rgb')
    
if __name__ == "__main__":
    mi.register_integrator("pt", lambda props: Pt(props))

    pt = mi.load_dict({
        'type' : 'pt',
        'max_depth' : 10,
        'rr_depth' : 5
    })

    scene = mi.load_file("./scenes/cornell-box/scene.xml")

    start = time.perf_counter()
    image = mi.render(scene, spp=16, integrator=pt)
    end = time.perf_counter()
    mi.util.write_bitmap("d5.png", image)
    print(f"Rendering time = {(end - start) * 1000} ms")