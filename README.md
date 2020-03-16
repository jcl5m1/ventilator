![Full System](https://github.com/jcl5m1/ventilator/blob/master/images/full%20system.jpg?raw=true)

# Low-Cost Arduino-Based Ventilator
In the event that COVID-19 hospitalizations exhaust the availability of FDA approved ventilators.  This project documents the process of converting a low-cost CPAP (Continuous Positive Airway Pressure) blower into a rudimentary Ventilator that could help with breathing during an acute respitory attack.  

<strong>WARNING/DISCLAIMER: Whenever possible, please seek professional medical care with proper equipment setup by trained individuals. Do not use random information you found on the internet. I am not a medical professional, just a random person on the internet.</strong>



# To Do's (aka. things that I could use help with):
1. A pulmonologist that (while understanding this is not an FDA approved device) is willing to advise on how to make this more effective as a last resort.
2. Testing with a simple inflator pump and achieve the same pressure control.
3. Can we detect the breath cycle by simply watching the motor current? Adding ait pressure sensors may complicate the build meaningfully.  But, using a sense Resistor is simple enough to try.
4. Developing a better solution to the mouth gaurd.  Current solution would require a bunch of tape to create a sufficent forward pressure.
5. If there is desire to make these, I don't have a supply chain setup. The few parts I have on hand may not be easily sourcable in any volume quickly.
6. Solutions to prevent the system from becoming unsafe if power is lost, is not obvious to me.  Ideas welcome.

To provide comments/feedback, create a new issue on the project.  Thanks!


# References about the supply and potential shortage of ventilators:

Here we will briefly summarize information on the supply and potential shortage.  Key numbers that will affect this:

1. The current numbers for supply of ventilators (# of ventilators currently)
2. The demand for ventilators, over time (# of ventilators needed over time)
3. The ability to increase supply of ventilators (# of ventilators able to be manufactured over time)

Additionally, the numbers above will vary based on each region's need globally.

Some preliminary data (we can continue to update) on some of these areas:

| Region          | # ventilators, current supply | # ventilators, needed  |
| -------------   |:-----------------------------:| --------------------: |
| Globe           |                               |                       |
| U.S.            | 46,000 [1]                    |   200,000 or more [1] |
| California      | 7,587 and "buying more" now [2]   |                       |
| ... add more... | ...         | ... |

References

- [1] Text from my friend who is a doctor at Stanford, let's replace with citable sources if possible. "Estimated 46,000 vent beds across US with anticipated need of 200,000, maybe more if more severe than we know"
- [2] Gavin Newsom press conference, March 15, 2020 (add link)


# References about CPAP/BiPAP/PEEP ventilator modes
[Lung Mechanics: PEEP,PEP,ACMV](https://www.youtube.com/watch?v=ScoSEeZJE08)

[Ventilator Modes Explained! PEEP, CPAP, Pressure vs. Volume](https://www.youtube.com/watch?v=iP_jN1qAPtI)


# Materials
1. [Continuous Positive Airway Pressure (CPAP)](https://en.wikipedia.org/wiki/Continuous_positive_airway_pressure) devices are common devices used by individuals to treat sleep apnea. As a result, CPAP blower components are fairly abundant. Used ones are available on eBay and new ones on Alibaba. In many of these devices, the blower is a simple BLDC motor that can be driven with an ESC.  The one I used is from a [Respironics REMStar](https://www.google.com/search?q=respironics+remstar&safe=off&rlz=1C1CHBF_enUS809US809&sxsrf=ALeKk00Uw98vXCAyCiiSglgt-2Ucnz5RCg:1584357572342&source=lnms&tbm=isch&sa=X&ved=2ahUKEwj2mq_y757oAhXOvJ4KHV22D1EQ_AUoAnoECAwQBA).  In volume, these can be under $20.  Potential alternative to test is a [12v inflator pump](https://www.amazon.com/gp/product/B013UQ0T2Y/ref=ppx_yo_dt_b_asin_title_o01_s00?ie=UTF8&psc=1) which is even more abundant.  

2.  [Arduino Compatible Nano/Clone](https://www.amazon.com/s?k=Arduino+nano) to control the motor speed, breathing cycles, and handle user input.   Download the [Arduino IDE](https://www.arduino.cc/en/main/software) to program the [microcontroller software](https://github.com/jcl5m1/ventilator/tree/master/ventilator_control).

3. Brushless DC Motor [Electronic Speed Controller](https://www.amazon.com/Hobbypower-Brushless-Controller-Multicopter-Helicopter/dp/B00XKX5TBE/ref=sr_1_17?keywords=12v+ESC&qid=1584357758&sr=8-17) capable of at least 10A. 

4. Button or swtich for input control.  The code cycles through 5 levels of pressure, and double click to swtich between CPAP and BiPAP/Fixed PEEP mode.

5. 12v DC power supply that can handle at least 5A.  The blower works pretty had, and the breathing cycles can create voltage drops which will reset the Arduino if the supply if not strong enough.

6. Tubing and face mask.  If you search for [CPAP face mask](https://www.google.com/search?q=cpap+mask), you'll find many options/suppliers.  The full face mask that covers both the mouth and nose and enabled pressure buildup in the lungs.  As an alternative, see below for draft 3D printable parts.

7. Optional [inline 12v battery backup](https://www.amazon.com/gp/product/B07H8F5HYJ/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1).

# Building
This is the full system, with mask, tubing, pump, 12v battery, and swtich for input.  However, I would strongly recommend running with a DC power supply since the battery will only last about 3-4 hours.  <strong>Wearing the mask with a dead battery could cause dangerous CO2 build up.</strong>  This [particular battery](https://www.amazon.com/gp/product/B07H8F5HYJ/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1) can charge and supply power at the same time, acting as an inline battery backup to wall power.

![Full System](https://github.com/jcl5m1/ventilator/blob/master/images/full%20system.jpg?raw=true)

Below is a close up of the extracted blower.  It simply an BLDC motor driving a blower fan.  A small ESC and Arduino attached (technically a Teensy2.0++).  It has a 3d printed tapered hose adpater on it to fit in the hose.  The [microcontroller software](https://github.com/jcl5m1/ventilator/tree/master/ventilator_control) can be programmed with the [Arduino IDE](https://www.arduino.cc/en/main/software). The software goes through a short initialization sequence to configure the ESC, and then starts at the lowest CPAP setting.  You can use the switch to cycle through pressure levels, and double click to cycle through operating modes.

![Extracted CPAP Blower](https://github.com/jcl5m1/ventilator/blob/master/images/IMG_20200315_230153.jpg?raw=true)

Since I have CPAP, I happen to have a [Resmed AitFit F20](https://www.amazon.com/Resmed-AirFit-Frame-System-Cushion/dp/B07C9MR5RS) mask that I can use.  This is quite comfortable and covers both the nose and mouth to make a good seal.  However if you can't get a mask,  below is a prototype mouthguard that fit in the category of not-so-great-but-usable-if-you-have-a-lot-of-tape.  [Here's the 3D model](https://github.com/jcl5m1/ventilator/blob/master/3dmodels/mouthgaurd_v20200316.1.stl).  This would require tape to create enough positive pressure.  A little bit of leakage is actually desirable to allow CO2 to escape near the mouth nose and flushes out the air over time, rather than being stuck in the tube.  <strong>WARNING: wearing this taped down without power may cause dangerous CO2 build up.</strong>

![Mouth Guard](https://github.com/jcl5m1/ventilator/blob/master/images/mouthguard.jpg?raw=true)
![Mouth Guard model](https://github.com/jcl5m1/ventilator/blob/master/images/mouth_guard_model.JPG?raw=true)


# Pressure Testing

This is a picture of my test rig to measure the output pressure.  It's a U-shaped tube with attached measuring tape to check displacement of water.  The actual liquid displacement is 2x the measured line since liquid is being push downwards on the other side of the U an equal amount, doubling the displacement. (I think this is the correct way to measure).  At full blast at 12v, it is achieving 45cm/H20.  Guidance on ventilators for COVID-19 patients require upto 20cm/H20.  So, these blowers are plently powerful.

![Pressure measurement fixture](https://github.com/jcl5m1/ventilator/blob/master/images/test_fixture.jpg?raw=true)

Here are some images showing the dislacement of the liquid at differen speeds.  I happen to have Diet Dr. Pepper on hand which makes it easier to see the liquid level rather than just plain water.

![3cm](https://github.com/jcl5m1/ventilator/blob/master/images/3cm.jpg?raw=true) ![6cm](https://github.com/jcl5m1/ventilator/blob/master/images/6cm.jpg?raw=true) ![11cm](https://github.com/jcl5m1/ventilator/blob/master/images/11cm.jpg?raw=true) ![22.5cm](https://github.com/jcl5m1/ventilator/blob/master/images/22.5.jpg?raw=true)

Here's a [video of the blower pushing a breathing cycle of 16 cycles/min oscilating between 12 and 22cm/H20](https://photos.app.goo.gl/b3yMPE5QpdeduxKS6).
