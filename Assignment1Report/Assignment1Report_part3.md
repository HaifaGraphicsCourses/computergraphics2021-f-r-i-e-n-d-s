# Branch 1
## Showing GIF of scaling and translating the model in the world frame
![GIF1](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/Scaling%26Translating2.gif)
## Showing GIF of rotating the model in the model frame in 360 degrees
![GIF1](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/LocalRotating.gif)
## ===================================================================
# Branch 2
## Choosing T1=Translate(500,0,0) and T2= Rotate in Y from 0-360 On banana model
### first we show the GIF displaying T1 in model frame and T2 in world frame
![GIF1](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/T1ModelT2World.gif)
### Now displaying the GIF where T1 is in world frame and T2 is in model frame
![GIF1](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/T1WorldT2Model.gif)
## ===================================================================
# Branch 3
## First we show a simple GIF showing the bounding box moving with the model and the reset button
![GIF1](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/BB&Reset.gif)
## The bellow GIF shows the normals per face(in purple) and normals per vertex (in white) and how they move with the model
![GIF1](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/NormalsGIF.gif)
## ===================================================================
# Branch 4
## First of all, we will show orthographic projection with low zoom and then with high zoom (orhto width is the parameter of the zooming).
### How i implemented the zooming parameter: given a new ortho width by the user we must maintain the original aspect ratio so what we do is:
### `new_width = ortho_width `
### `new_height = ortho_width/aspectRatio`
### and ofcourse `left= -new_width/2 , right= new_width/2 , top=new_height/2 , bottom=-new_height/2`;
 *In all the pictures the bounding box is beeing drawn in red in order to see the difference between the orthographic and perspective.*
 
![OrthoWide](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/OrhtoWide.png)

![OrthoZoom](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/OrthoZoom.png)

## Now We will see the perspective projection first with low zoom then with high zoom.

![PerspectiveWide](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/PerspectiveWide.png)

![PerspectiveZoom](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/PerspectiveZoom.png)

##### If we take a look at the bounding box we would see the difference between the orthographic projection and the perspective which is in the perspective we can see what happens on the z axis. for example applying TranslateModel(0,0,200) in the model frame on perspective projection would show us that the object is "getting closer",while on the other side applying the same transformation on the orthographic projection wouldn't show us anything.
## ===================================================================
# Branch 5
*The Axes are: X drawn in white and Y drawn in yello.*
#### First of all zooming is being done on the picutre by changing the fovy value. Then we translate the camera in Z. this difference is because the camera's location is on 0.2,0.2,0.2 which means eye=(0.2, 0.2, 0.2) but if we change it to (0,0,0.5) then there is no difference between translation in z axis and zooming using the fovy.
![ZoomingVsDistance](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/Branch5.gif)
## ===================================================================
# Branch 6
![Branch6](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/Branch6.gif)
## ===================================================================
# Branch 7
![SetCamera](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/SetCamera(aaa).JPG)
## ===================================================================
# Branch 9
#### First picture is the starting case and the second is after resizing the window shows how the model maintains its aspect ratio and stays in the center of the screen
![BeforeResize](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/BeforeResize.png)
![AfterResize](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/AfterResize.png)
## ===================================================================
# Branch 10


