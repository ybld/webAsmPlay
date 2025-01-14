----------------------
WebAsmPlay
----------------------

WebAsmPlay was initially developed to evaluate web application development using Emscripten.
Currently the Emscripten part is not working. Hopefully in the future it will be. 

[![Alt text](https://img.youtube.com/vi/s0unMIQUs1U/0.jpg)](https://www.youtube.com/watch?v=s0unMIQUs1U)<br/>
YouTube video of current native development version.

To see the last Emscripten build working: https://trailcode.github.io/ZombiGeoSim/index.html Be patient to let it load.
Hold down the "alt" key and move the mouse to pan. Hold down the "shift" key and move the mouse to rotate the camera.

The current goal of this project is morphing into a Real-Time Strategy game using
OpenSteer (http://opensteer.sourceforge.net/) and OpenStreetMap (https://www.openstreetmap.org)
 
### Features

* Ability to run the client in a supported web browser or as a native client. 
    * Object Picking where the picked object under the mouse cursor is highlighted and it's attributes are displayed.
    * Skybox Render.
    * Path finding over linear features.
    * Autonomous zombies wonder on paths while steering to avoid each other.
    * Multiple camera modes: TrackBall and zombie tracking camera mode.
    * User definable symbology color rendering supporting transparency for both fill and outlines.
        * Camera near and far symbology can be defined for each attribute allowing for interpolated symbology blending proportional to object to camera distance.
    * Ability to render 3D buildings.
    * Projection of BingMaps raster tiles over scene geometry. 
    * Create and play camera animation paths.

* Geometry Server
    * ESRI Shapefile and OpenStreetMap XML GIS formats with linestring and polygon ingestion along with attribution.
    * Fast custom attributed transport protocol.
    * Ensures linear feature topology correctness. Breaks linestrings at intersections and connections. Removes overlapping linestrings.
    * Discover polygon topographical relations. Child(contained), parent, and neighbors. 

### Sources

Some third party dependencies are built into the WebAsmPlay source tree to make emscripten compilation easer
and allow C++ 17 compilation:  
* Geos: https://trac.osgeo.org/geos
* OpenSteer: http://opensteer.sourceforge.net
* Ctpl: https://github.com/vit-vit/CTPL
* Glm: https://glm.g-truc.net/0.9.9/index.html
* GLUTessellator: https://github.com/mbebenita/GLUTesselator
* Native Dear Imgui: https://github.com/ocornut/imgui [Docking branch]
* Imgui-addons: https://github.com/wflohry/imgui-addons
* Emscripten Dear Imgui: https://github.com/greggman/doodles/tree/master/glfw-imgui
* Websocketpp: https://github.com/zaphoyd/websocketpp
* Histogram: https://github.com/HDembinski/histogram
* SimpleJSON: https://github.com/MJPA/SimpleJSON

#### CUDA
* https://docs.nvidia.com/deeplearning/sdk/cudnn-install/index.html

#### Building

* Install Python37
  * pip install --user numpy
    * The --user flag is required for permission issues. 
* Install boost
  * Build with python support, must find numpy when building
    * b2.exe address-model=64 architecture=x86 link=shared threading=multi runtime-link=shared --with-chrono --with-date_time --with-filesystem --with-system --with-atomic --with-thread --with-timer --with-program_options --with-python --variant=release,debug --build-type=minimal stage -j8
* pip3 install tensorflow-gpu // Cannot get this one to work. 
  * pip3 install tf-nightly-gpu 
* pip3 install matplotlib
* pip3 install Pillow
* pip3 install ipython

The build system is based on CMake. 

### OSX

* brew install boost
* brew install glfw3
    * brew link glfw
* brew install expat keg-only does not link in /usr/local
* brew install assimp
* brew install sdl2_image

#### Bugs

* ~~dmessError does not flush output~~

#### Todo

* Replace curlUtil::BufferStruct with a std::pair
* Remove the model component from the canvas, simplify the shaders.
* Light pos for PhongShader, PhongShaderInstanced, ColorDistanceDepthShader3D and ColorDistanceShader is inconsistent. 
* Unify textures. Bing map tiles and bubble tiles. Create mechanism to prune tile textures from GPU.
* Models need to work with SSAO pipeline.
* Add ctpl thread pool to CMakeLists.txt so it is editable.
* Create dedicated threadpools for file IO.
* Search for thread_pool and ensure correct implementation for stack or queue depending on usage
  * Consider priority queue system for bing tiles and street side tiles
  * Ensure thread pools are shutting down correctly.
* Create a performance watchdog to break if a frame is taking too long to render.
* Look into https://www.bing.com/api/maps/sdk/mapcontrol/isdk/birdseyev2 for projecting onto the sides of buildings. 
* // TODO Use an array! No newsing and deleting all the time.
* Look into https://www.mapillary.com/
  * https://mapillary.github.io/mapillary_solutions/data-request/
  * OSM ID editor shows "Map Features" and "Traffic Signs"
* Look into https://openstreetcam.org/
* Think about using street signs.
* DeferredRenderable::createFromQueued needs cleanup


#### Notes

* https://docs.microsoft.com/en-us/bingmaps/articles/getting-streetside-tiles-from-imagery-metadata
* https://docs.microsoft.com/en-us/bingmaps/rest-services/imagery/imagery-metadata

* https://gamedev.stackexchange.com/questions/19461/opengl-glsl-render-to-cube-map

* TensorFlow Notes
  * https://medium.com/@WuStangDan/step-by-step-tensorflow-object-detection-api-tutorial-part-1-selecting-a-model-a02b6aabe39e
  * At time of writing requires this CUDA toolkit: https://developer.nvidia.com/cuda-10.0-download-archive
  * pycocotools windows: ```pip install pycocotools``` fails
    * pip install --upgrade cython
    * pip install "git+https://github.com/philferriere/cocoapi.git#egg=pycocotools&subdirectory=PythonAPI"
  * set PYTHONPATH=%PYTHONPATH%;C:\src\models\research;C:\src\models\research\slim
  * Read and try this: https://github.com/Allopart/Maritme_Mask_RCNN

