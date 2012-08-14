KINACTOR PROJECT
=======================

APPS
-----------------------

* KinActor: get kinect signal with ofxKinect and ofxOpenCv
* OniActor: get kinect Skeletons with ofxOpenNI and send with ofxOsc
* OniTheatrePrototypes: receive OSC Skeletons in Unity3D and more ... (da qui facciamo un'app template per il workshop)

DIRS
-----------------------

* bin: put here external binaries/apps for testing
* doc: put here documentation
* video: put here videos

INSTALLATION OSX
=======================

- download openFrameworks 0071 OSX release: http://www.openframeworks.cc/versions/v0.071/of_0071_osx_release.zip

- unzip openFrameworks somewhere

- INSTALL REQUIRED ADDITIONAL ADDONS

<pre>
cd of_0071_osx_release/addons
git clone https://github.com/ofTheo/ofxKinect
git clone https://github.com/rezaali/ofxUI
git clone https://github.com/gameoverhack/ofxOpenNI/
</pre>

- MAKE A DIRECTORY FOR YOUR PROJECTS AND CLONE THIS PROJECT THERE

<pre>
cd of_0071_osx_release
mkdir Code
cd Code
git clone https://github.com/RecipientCollective/KinActor_project.git
</pre>

- BUILD IN XCODE 4


REQUIREMENTS
=======================

* XCODE 4

* OPENFRAMEWORKS 0071 OSX release

* ADDONS

	* ofxKinect https://github.com/ofTheo/ofxKinect

	* ofxOpenCv https://github.com/openframeworks/ofxOpenCv

	* ofxUI  https://github.com/rezaali/ofxUI

	* ofxOpenNI https://github.com/gameoverhack/ofxOpenNI/



REPOSITORY STRUCTURE
=======================

* BRANCHES

	* example_project --> test di ofxKinect and openCV

	* experimental --> experiments ...

