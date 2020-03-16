# Arduino-Based Ventilator
In the event that COVID-19 hospitalizations Exhaust the availability of FDA approved ventilators.  This project documents the process of converting a low-cost CPAP (Continuous Positive Airway Pressure) blower into a rudimentary Ventilator that could help with breathing during an acute respitory attack.  

Whenever possible, please seek professional medical care with proper equipment setup by trained individuals. Do not use random information you found on the internet.  

# References about CPAP/BiPAP/PEEP ventilator modes
[Lung Mechanics: PEEP,PEP,ACMV](https://www.youtube.com/watch?v=ScoSEeZJE08)

[Ventilator Modes Explained! PEEP, CPAP, Pressure vs. Volume](https://www.youtube.com/watch?v=iP_jN1qAPtI)


# Materials
1. [Continuous Positive Airway Pressure (CPAP)](https://en.wikipedia.org/wiki/Continuous_positive_airway_pressure) devices are fairly common devices used by individuals to treat sleep apnea. As a result, CPAP blower components are fairly abundant. Used ones are available on eBay, and new ones on Alibaba. In many of these devices, the blower is a simple BLDC motors that can be driven with a simple ESC.  The one I used i from a [Respironics REMStar](https://www.google.com/search?q=respironics+remstar&safe=off&rlz=1C1CHBF_enUS809US809&sxsrf=ALeKk00Uw98vXCAyCiiSglgt-2Ucnz5RCg:1584357572342&source=lnms&tbm=isch&sa=X&ved=2ahUKEwj2mq_y757oAhXOvJ4KHV22D1EQ_AUoAnoECAwQBA).  In volume, these can be under $20.

2.  [Arduino Nano](https://www.amazon.com/s?k=Arduino+nano) to control the motor speed, breating cycles, and take user input.

3. Brushless DC Motor [Electronic Speed Controller](https://www.amazon.com/Hobbypower-Brushless-Controller-Multicopter-Helicopter/dp/B00XKX5TBE/ref=sr_1_17?keywords=12v+ESC&qid=1584357758&sr=8-17) capable of at least 10A. 

4. Button or swtich for input.

5. 12v DC power supply that can handle at least 5A.  The blower works pretty had, and the breathing cycles can create voltage drops which will reset the Arduino if the supply if not strong enough.

6. Tubing and face mask.  If you search for [CPAP face mask](https://www.google.com/search?q=cpap+mask), you'll find many options/suppliers.  I recommend a full face mask that covers both the mouth and nose.

# Building
This is what the extrated blower looks like with the ESC and Arduino attached (technically a Teensy2.0++).

![Extracted CPAP Blower](https://github.com/jcl5m1/ventilator/blob/master/images/IMG_20200315_230153.jpg?raw=true)

# Testing

This is a picture of my test rig to measure the output pressure.  It's a U shaped tube with attached measuring tape to check displacement of water.  The actually displacement is 2x the measured lin because the liquid is being push downwards on the other side of the U an equal amount.  At full blast at 12v, it is achieving 45cm/H20.  Guidance on ventilators for COVID-19 patients require upto 20cm/H20.  So, this blower is plently.

![Pressure measurement fixture](https://github.com/jcl5m1/ventilator/blob/master/images/test_fixture.jpg?raw=true)

Here are some images showing the dislacement of the liquid.  I happen to have Diet Dr. Pepper on hand which is easier to see the level than just water.

![3cm](https://github.com/jcl5m1/ventilator/blob/master/images/3cm.jpg?raw=true)

![11cm](https://github.com/jcl5m1/ventilator/blob/master/images/11cm.jpg?raw=true)

![22.5cm](https://github.com/jcl5m1/ventilator/blob/master/images/22.5.jpg?raw=true)

Here's a [video of the blower pushing a breathing cycle of 16 cycles/min oscilating between 12 and 22cm/H20](https://photos.app.goo.gl/b3yMPE5QpdeduxKS6).
