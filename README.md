## Cornea plugin for Vaa3d
Build:

`qmake`

`make`

Call:

`vaa3d_build -x cornea_tracing_plugin -f image_thresholding -i ~/projects/cornea_project/DATA_example_Holco_praire_Scan29/processed/volumes/C3-Holco_Scan29_scaled.tif -o output_image.tif -p 0 100`

`vaa3d_build -x cornea_tracing_plugin -f full_tracing -i ~/projects/cornea_project/DATA_example+Holco_praire_Scan29/processed/volumes/C3-Holco_Scan29_scaled.tif -o output_image.tif -p 5 3`

Comments: 

* running on full 3.6GB file runs out of RAM on 8GB laptop (it works on downsampled or cropped versions)
* for small RAM machines, it is best to downsample the image using a separate resampling plugin:

` vaa3d_build -x image_resample -f down_sample -i ~/projects/cornea_project/DATA_example_Holco_praire_Scan29/processed/volumes/Holco_Scan29_resampled.v3draw -o small_image.tif -p 2 2 2`

After this procedure most probably one would want to use an upsampling procedure to upsample the final .swc`


* tested on macOS Sierra 10.12.5, to run on a different mac 
	* copy bin folder to new computer
	* run 
`./bin/vaa3d64.app/Contents/MacOS/vaa3d64 -x cornea_tracing_plugin -f full_tracing -i ~/projects/cornea_project/DATA_example_Holco_praire_Scan29/processed/volumes/C3-Holco_Scan29_scaled.tif -o output_image.tif -p 5 3`
