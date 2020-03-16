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
5. Tubing and face mask.  If you search for [CPAP face mask](https://www.google.com/search?q=cpap+mask), you'll find many options/suppliers.  I recommend a full face mask that covers both the mouth and nose.

# Building
This is what the extrated blower looks like with the ESC and Arduino attached (technically a Teensy2.0++).

![Extracted CPAP Blower](https://lh3.googleusercontent.com/hPX3GiEjg27p6zlAJaIuewPsdLUfEzfqP5ohYC8VrtviFaYc5ei6jfTAS4LIBDdcHIdhpuq_E_aBKdyIbA0rODisRbDJCsFfAkan4rW36lzpe-3EGOYdA0jlX49WbZHJcepkXvDF1P0NWBhbsMosDWOsoL--PNAMOa0Y7Pk5AOKd6payBJhetiSbAD9iaavFTFQ5_ocNvrmUvQWIh7D92lYEzs09eLpB6oVJHv72387tul4qr0XIMQGp-V1sKZyPnIUXcOpnQj_iSyf39G_IHyFqIEp7XW8RS-CmWnMwjrGZbokv5Tm6w1S-3qRA0ivpDzjrzTzSzmyzhJ-L9B8HyR7URa23Q4meFWTSxA2hmK6O6EXueTxm1Q7RPoEQTbSH99uxQOb9FyVSBnoQABrHeudZbMujljwKpZpmsGyaIoxmAizKf-WFFApqdgIrUoMPhP6W00xGlSxr4rzU2taDTbNcBUuWDMOIUhMpQsnCqzDIboH4UKPNTyqL7qyJssZLCE_nFJ4u3074DjW5RNW1HRNGahOXi7TjbP9tHHyY2TCor28YiaPTaT777wy8swnhyHBhXFfz-bpylixZSGWibXfg-sT9b0Ytqj14yTBwZUx0zusSp7644Dgo2-wZyADEsb-xVDc8H0_f1qCgVn4XAA-p_VsYbTm-YZrOQ-ceHtljD4Y2A6WvNlFMAzSp_g=w817-h1089-no)

# Testing

This is a picture of my test rig to measure the output pressure.  It's a U shaped tube with attached measuring tape to check displacement of water.  The actually displacement is 2x the measured lin because the liquid is being push downwards on the other side of the U an equal amount.  At full blast at 12v, it is achieving 45cm/H20.  Guidance on ventilators for COVID-19 patients require upto 20cm/H20.  So, this blower is plently.

![Pressure measurement fixture](https://photos.app.goo.gl/yFZ8Rt3BpK8DESDU7)

Here are some images showing the dislacement of the liquid.  I happen to have Diet Dr. Pepper on hand which is easier to see the level than just water.

![Zero Level](https://photos.app.goo.gl/veUevtJsQ1AmYuKt9)

![3cm](https://photos.app.goo.gl/X6HGR8Wkmg5hVmJJ6)

![6cm](https://photos.app.goo.gl/BLHT4Zwp7VhZBJxu7)

![11cm](https://photos.app.goo.gl/8LS1PzBmErVFTzZf7)

![17cm](https://photos.app.goo.gl/ZGvVcYphmSGSGjmh9)

![22.5cm](https://photos.app.goo.gl/ZCUHTp5oduq7fScm7)

Here's a [video of the blower pushing a breathing cycle of 16 cycles/min oscilating between 12 and 22cm/H20](https://photos.app.goo.gl/b3yMPE5QpdeduxKS6).
