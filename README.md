# Arduino-Based Ventilator
In the event that COVID-19 hospitalizations exhaust the availability of FDA approved ventilators.  This project documents the process of converting a low-cost CPAP (Continuous Positive Airway Pressure) blower into a rudimentary Ventilator that could help with breathing during an acute respitory attack.  

Whenever possible, please seek professional medical care with proper equipment setup by trained individuals. Do not use random information you found on the internet.  

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

2.  [Arduino Nano](https://www.amazon.com/s?k=Arduino+nano) to control the motor speed, breathing cycles, and handle user input.

3. Brushless DC Motor [Electronic Speed Controller](https://www.amazon.com/Hobbypower-Brushless-Controller-Multicopter-Helicopter/dp/B00XKX5TBE/ref=sr_1_17?keywords=12v+ESC&qid=1584357758&sr=8-17) capable of at least 10A. 

4. Button or swtich for input control.  The code cycles through 5 levels of pressure, and double click to swtich between CPAP and BiPAP/Fixed PEEP mode.

5. 12v DC power supply that can handle at least 5A.  The blower works pretty had, and the breathing cycles can create voltage drops which will reset the Arduino if the supply if not strong enough.

6. Tubing and face mask.  If you search for [CPAP face mask](https://www.google.com/search?q=cpap+mask), you'll find many options/suppliers.  I recommend a full face mask that covers both the mouth and nose.  As an alternative, see below for draft 3D printable parts.


# Building
This is the full system, with mask, tubing, pump, 12v battery, and swtich for input.
![Full System]()

This is a close up of the extracted blower.  It simply an BLDC motor driving a blower fan.  A small ESC and Arduino attached (technically a Teensy2.0++).  It has a 3d printed tapered hose adpater on it to fit in the hose.

![Extracted CPAP Blower](https://github.com/jcl5m1/ventilator/blob/master/images/IMG_20200315_230153.jpg?raw=true)

Since I have CPAP, I happen to have a [Resmed AitFit F20](https://www.amazon.com/Resmed-AirFit-Frame-System-Cushion/dp/B07C9MR5RS) mask mask that I can use.  This is quite comfortable, and covers both the nose and mouth to make agood seal.  However, below is a prototype mouthguard (with some tape), could provide a not great but usable solution.  [Here's the 3D model](https://github.com/jcl5m1/ventilator/blob/master/3dmodels/mouthgaurd_v20200316.1.stl).

![Mouth Guard](https://github.com/jcl5m1/ventilator/blob/master/images/mouthguard.jpg?raw=true)
![Mouth Guard model](https://github.com/jcl5m1/ventilator/blob/master/images/mouth_guard_model.JPG?raw=true)


# Pressure Testing

This is a picture of my test rig to measure the output pressure.  It's a U-shaped tube with attached measuring tape to check displacement of water.  The actual liquid displacement is 2x the measured line since liquid is being push downwards on the other side of the U an equal amount, doubling the displacement. (I think this is the correct way to measure).  At full blast at 12v, it is achieving 45cm/H20.  Guidance on ventilators for COVID-19 patients require upto 20cm/H20.  So, these blowers are plently powerful.

![Pressure measurement fixture](https://github.com/jcl5m1/ventilator/blob/master/images/test_fixture.jpg?raw=true)

Here are some images showing the dislacement of the liquid at differen speeds.  I happen to have Diet Dr. Pepper on hand which makes it easier to see the liquid level rather than just plain water.

![3cm](https://github.com/jcl5m1/ventilator/blob/master/images/3cm.jpg?raw=true) ![6cm](https://github.com/jcl5m1/ventilator/blob/master/images/6cm.jpg?raw=true) ![11cm](https://github.com/jcl5m1/ventilator/blob/master/images/11cm.jpg?raw=true) ![22.5cm](https://github.com/jcl5m1/ventilator/blob/master/images/22.5.jpg?raw=true)

Here's a [video of the blower pushing a breathing cycle of 16 cycles/min oscilating between 12 and 22cm/H20](https://photos.app.goo.gl/b3yMPE5QpdeduxKS6).
