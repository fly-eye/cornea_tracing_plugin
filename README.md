## Example plugin for Vaa3d
Build:
qmake
make

Call:

vaa3d_build -x example_plugin -f image_thresholding -i ~/projects/cornea\ project/DATA_example_Holco_praire_Scan29/C3-Holco_Scan29.v3draw -o output_image.tif -p 0 100

vaa3d_build -x example_plugin -f simple_adaptive_thresholding -i ~/projects/cornea\ project/DATA_example+Holco_praire_Scan29/C3-Holco_Scan29.v3draw -o output_image.tif -p 5 3

Comments: 

* there was some issue with tif file, so switched to v3draw (need to reverify if it works with tif).
* running on full 3.6GB file runs out of RAM on 8GB laptop (it works on cropped version)
