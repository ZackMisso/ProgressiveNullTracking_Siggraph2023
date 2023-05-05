This readme will be populated soon with specific build instructions, but for now:

simple/ contains the code for generating all graphs in our paper within simple/prog_scripts/, as well as a small testbed for testing transmittance estimators for simple 1D problems.

pbrt/ contains our implementation within the PBRTv3 renderer.

To get our scene data such as the cloud or smoke vdb files, please email me (Zack) at zack441@mac.com. I am currently still trying to figure out how best to share the data since they are too big to include in a git commit.

All figures can more or less be recreated by running either the pbrt project or simple project. Each project has a folder called experiments/ within the src folder which contains isolated c++ files to generate sample results from specific figures.
