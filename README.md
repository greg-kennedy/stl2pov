# stl2pov
Convert 3d model .stl files to POV-Ray .pov format

## Usage
./stl2pov input.stl > output.pov

## Detail
The output files are in POV-Ray "mesh2" format, with one declared macro named "stl_<filename>" that contains the entire model.

For example, if you've converted `3DBenchy.stl`, this sample POV scene can import and render it:

```
#version 3.7;
global_settings {
  assumed_gamma 1.0
}

#include "3DBenchy.pov"

camera {
  location <50,50,50>
  look_at <0,25,0>
}

light_source {
  <25, 50, 25> rgb<1, 1, 1>
}

object {
  stl_3DBenchy
  rotate x * -90
  pigment { rgb<1, 1, 0> }
}
```

Deduplication of vertices and triangles is performed during conversion, this may lead to reported vs actual triangle_count being different.
