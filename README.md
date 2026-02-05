MidiMarble
------------

I purchased an Akai MidiMix to use to map a vst in my DAW and I liked it so much that I wanted another but I'm cheap so i thought it would be a fun project to make.

I'm not a coder so I looked to the internet for something that i could just DIY.  Sadly there were very few 32 pot controller projects.  The closest was the MidiMood project by github user silveirago (or Nerd Musician on YT).  Here's his Moog Mini vst controller that was a big inspiration.  I definitely suggest you check him out if you haven't already.  He does a lot of cool midi controllers on his YT and even has a training course on making your own on his website.  Check out his project here:
https://github.com/silveirago/MidiMood

So I did the classic "i bet i can make one of those too"  Well as I said I'm no coder.  I have hobbyist level electronic skills and i'm good with an iron so I did what any other newb would do and hop on chatgpt to get some vibe code going.  A few hours later i had achieved a complete mess of a sketch and i was pretty sure I was way in over my head.  But i dialed back all the "cool features I wanted" like per knob calibration routine, midi channel signal LED's and even an on-mcu midi map that could be called up with a selector on the hardware.  I got too ambitious and so i scrapped that code for the most part.  I took the basic parts and this repo is going to be a WIP for this.

Like most hobbyist gitters I'll probably abandon this project in a week or 2 but feel free to use this code if you find it useful or inspirational.

It's under the MIT license so you are free to use any or all of it in your own personal projects (not for sale) and all I ask is attribution credit in your comment intro block.

I used the GroundStudio Marble Pico which is a (clone) of a raspberry pi pico 2040.  It uses legit rpi chips but it's in a teensy footprint in a 23 pin U shape.  It was cheap on Amazon so i picked up a handful and i like the form factor.  It has 4 ADC's 16+ Digital GPIO.  It's perfect for a modest midi controller.  And of course supports midi class compliant out of the box.  

I coded my project in Arduino IDE and I have a little bit of working knowledge of scripting in it.  I'm sure circuitpython is as good or better but i am so far able to do what I needed so for now it's perfectly suitable.

I used 2 CD74HC4067 mux's to get me the 32 analog ins I needed for the 32 10k pots I used.

Nothing fancy but it works surprisingly well for the couple hours I spent with the AI code box.

More to come as I do more with it,  I am thinking i want to add 1 or 2 hall effect joysticks to be used to map macros of different vst effect controls.  LIke a combo resonance filter/cutoff or to morph a chorus and a reverb or distortion effect for some interesting automation in my music.  Other than that, maybe an on/off switch, couple LED indicators for midi cc send to make it feel more connected.  I have some ec11 rotary encoders in my parts bin.  I might use a few of those too for infinite encoder-tasks.

Thanks for stopping by and checking out my little project.  Here's a couple of quick pictures from my build progress:
![IMG_0538](https://github.com/user-attachments/assets/e596864f-5243-4574-8325-19bc44957451)
![74B976A0-733B-424F-88B1-7D5BB05077BA](https://github.com/user-attachments/assets/a8dc6b42-e8d2-4513-86d0-b5e6bed2fa76)
![IMG_0535](https://github.com/user-attachments/assets/54da876e-4951-4de4-9b58-e38916b2b637)
![IMG_0537](https://github.com/user-attachments/assets/e1c9d428-395b-4a64-a721-f6c8ea39d32f)
