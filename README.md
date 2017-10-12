## Cornea tracing plugin for Vaa3d
Build:

`qmake`

`make`

Call:

image_thresholding

`vaa3d_build -x cornea_tracing_plugin -f image_thresholding -i ~/projects/cornea_project/DATA_example_Holco_praire_Scan29/processed/volumes/C3-Holco_Scan29_scaled.tif -o output_image.tif -p 0 100`

cornea_tracing_plugin

`vaa3d_build -x cornea_tracing_plugin -f full_tracing -i input_image.tif -o output_image.tif -p h d c`

* input_image (the image to process - it can be .v3draw or .tif)
* output_image (stores the thresholded image)
* h - sampling interval in adaptive image thresholding (h = 5)
* d - number of sampling points in adaptive image thresholding (d = 3)
* c - the channel to trace (c=1)

The final tracing is stored in a file input_image_SIGEN.swc.

Example: 

`vaa3d_build -x cornea_tracing_plugin -f full_tracing -i ~/projects/cornea_project/DATA_example_Holco_praire_Scan29/processed/volumes/C3-Holco_Scan29_scaled.tif -o output_image.tif -p 5 3`




Comments: 

* running on full 3.6GB file runs out of RAM on 8GB laptop (it works on downsampled or cropped versions)
* for small RAM machines, it is best to downsample the image using a separate resampling plugin:

`vaa3d_build -x image_resample -f down_sample -i ~/projects/cornea_project/DATA_example_Holco_praire_Scan29/processed/volumes/Holco_Scan29_resampled.v3draw -o small_image.tif -p 2 2 2`

Note that the factor is always greater than 1: what changes is the function: up_sample/down_sample. 

After this procedure most probably one would want to use an upsampling procedure to upsample the final .swc. (with resample_swc plugin).

If data is in a folder of tif files, convert to tif stack by

`vaa3d_build -x IVSCC_import_data -f import -i /Users/valentina/projects/cornea_project/Holco_Scan53_fly1_eye1 -o ~/projects/cornea_project/Holco_Scan53_fly1_eye1/combined53.v3draw  -p 0 0
`

(before that need tp build the plugin in vaa3d_tools/hackathon/zhi/IVSCC_import_data)

* tested on macOS Sierra 10.12.5, to run on a different mac 
	* copy bin folder to new computer
	* run 
`./bin/vaa3d64.app/Contents/MacOS/vaa3d64 -x cornea_tracing_plugin -f full_tracing -i ~/projects/cornea_project/DATA_example_Holco_praire_Scan29/processed/volumes/C3-Holco_Scan29_scaled.tif -o output_image.tif -p 5 3`

