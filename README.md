PIF2XYZ
=======

Converts point clouds in InnovMetric Parametric Image Format (PIF) to standard ASCII XYZ.

Usage
-----

```
pif2xyz.exe source.pf destination.xyz
```

Sample output:

```
format_version        PIF Format v2.0
user_comments         Optech ILRIS-3D
image_param_flag      0
image_data_type       1
invalid_point         0.000000
array_width           537
array_height          651
data_block_length     4195044
scale_flag            0
i_scale               0
j_scale               0
transfo_matrix_flag   2
transfo_matrix
                      1.00 0.00 0.00 0.00
                      0.00 0.00 -1.00 0.00
                      0.00 1.00 0.00 0.00
                      0.00 0.00 0.00 1.00

image_color_flag      3
color_block_length    1048761
camera_position_flag  1
camera_x              0.000000
camera_y              0.000000
camera_z              0.000000
```

