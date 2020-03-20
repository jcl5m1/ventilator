Last updated 2020/03/20

![Full System](https://github.com/jcl5m1/ventilator/blob/master/images/full%20system.jpg?raw=true)

# Low-Cost Open-Source Ventilator-ish Device (or a PAPR)
In the event that COVID-19 hospitalizations exhaust the availability of FDA approved ventilators.  This project documents the process of converting a low-cost CPAP (Continuous Positive Airway Pressure) blower into a rudimentary ventilator that could help with breathing during an acute respiratory attack.  

### WARNING/DISCLAIMER: Whenever possible, please seek professional medical care with proper equipment setup by trained individuals. Do not use random information you found on the internet. I am not a medical professional, just a random person on the internet. There are significant risks associated with using a high pressure BiPAP as a DIY ventilator without medical supervision.  

### Alternative: Build a low-cost PAPR!
As an alternative to building a mediocre DIY ventilator, this device can also become a reasonable [low-cost Powered Air Purifying Respirator (PAPR)](https://github.com/jcl5m1/ventilator/wiki/Build-a-Low-Cost-PAPR) with filter adapter+mask.  PAPRs are effective pieces of Personal Protective Equipment (PPE) that could be helpful to caregivers. PAPRs will also be in extremely short supply and are much less risky and less controversial than building a DIY Ventliator.  You may be able to save many more lives by building a PAPR that protects a caregiver than a medicore ventilator for one patient.

### What are the primary risks of a DIY ventilator?
 * Being viewed as a viable alternative to available professional care or delaying seeking professional care.  This is should be view as educational resource or a last resort option only.  Community driven engineering efforts like this run the risk of making it "too easy to be harmful" which would be a bad outcome.
 * Using non-invasive interfaces (such as masks and mouth pieces) can create aerosolized virus infecting many others in clean environments.  Infecing yourself or other healthy individuals multiplies the problem.  Restrict usage to places where aerosolized viruses are already abundant or in isolation. <strong>NOTE: Self-quarantine in an apartment or multi-unit residence is NOT proper isolation due to potential shared ventilation and sewage.</strong>
* Giving too high a pressure (such as 20cmh20 and up) without observing lung compliance can cause rupture of infected lung tissue.  This design currently lacks several features that to provide feedback to a caregiver to monitor that risk.
* See [more detailed discussion of these risk and potential engineering/administrative controls to address them](https://github.com/jcl5m1/ventilator/wiki/Notes-from-chatting-with-a-pulmonologist).

### Hardware Capabilities
This rudimentary design could provide a programmable target breathing rate (10-16/min), a target Positive End-Expiratory Pressure (PEEP), and the blower is physicially able to hit a peak airway pressure of upto 45cmH20. However, <strong>[going above 20cmH20 can be DANGEROUS](https://github.com/jcl5m1/ventilator/wiki/Notes-from-chatting-with-a-pulmonologist)</strong>.  It can't guarantee a specific tidal volume, and doesn't regulate specific fractional oxygen (FiO2).  It is very doubtfull the blower is safe for 100% O2, so it just pushes atmospheric air (21% O2). Any other ratio would require O2 feed down stream and manual guess work supply flow.  It is lacking in many ways, but it is possible this limited functionality could help when better care is not available.

Here's a youtube video walking through the components as of 2020/03/17:
[![Youtube video](https://img.youtube.com/vi/n57u1NvXBgw/0.jpg)](https://www.youtube.com/watch?v=n57u1NvXBgw)

There are also many other efforts in this direction.  [Here's a short list of them](https://github.com/jcl5m1/ventilator/blob/master/resources.md) Thanks! [discussion](https://github.com/jcl5m1/ventilator/pull/20) 

# To Do's (aka. things that I could use help with):
1. I had a long conversation with a pulmonologist to discuss the critical features of a proper ventilator and trade offs. [Here are my notes](https://github.com/jcl5m1/ventilator/wiki/Notes-from-chatting-with-a-pulmonologist).  If you know someone with domain expertise, I would like confirmation that my take away notes are reasonable since this will drive priorities on engineering effort/conversations.
2. Develop a simple low-cost sensor configuration that can [provide a similar approximation of lung compliance provided by a ventilator loop display](https://github.com/jcl5m1/ventilator/wiki/Notes-from-chatting-with-a-pulmonologist#at-a-high-level-the-goal-is-to-get-enough-air-into-the-lungs-to-keep-the-patient-alive-without-causing-unnecessary-stress-on-infected-lung-tissue)
3. Test lung sourcing/research.  Example high end one from [Michigan Instruments](https://www.michiganinstruments.com/lung-simulators/adult-test-lung-simulators/).
4. A supply chain person that can help us find good suppliers of parts. If there is desire to make these, I don't have a supply chain setup. The few parts I have on hand may not be easily sourcable in any volume quickly.
5. Figure out how to reprogram an existing BiPAP. If this operating mode is helpful, there's a large number of these device's already in the field which could be enabled.  I don't have good stats on the popularity of different makes/models.  There could be on the order of a [1M devices](https://www.cheapcpapsupplies.com/blog/sleep-apnea-statistics/) worldwide?
6. Design modifications to recapture exhaust to minimze addition viral spread. This only works in already "dirty" environments, where there is likely already airborne droplets containing virus - such as quarnateened spaces or other negative air pressure environments.
7. Develop Arduino compatible algorithms to align blower cycle to the breathing cycle by [looking at backpressure load on the blower](https://github.com/jcl5m1/ventilator/wiki/Breathing-Back-Pressure-Data-Analysis), or very simple air pressure sensor that can be inserted into the output of the pump.  For ease of developing algorithms see [Colab notebook with data visualization.](https://colab.research.google.com/drive/1iFMmMMrg_3OeifzJikT75fy4ev84W235)
8. Solutions to prevent the system from becoming unsafe if power is lost.  It's not obvious to me on how to do this.  Ideas are welcome.
9. A 3D model for an [inline air filter](https://www.directhomemedical.com/cart/merchant.mvc?Screen=PROD&Product_Code=1-H1605-inline-bacterial-viral-cpap-filters&Store_Code=DHM&gclid=EAIaIQobChMIo-G35omi6AIVSrzACh29hwv3EAQYASABEgK9FPD_BwE) with link to common filter supply [such as this](https://www.amazon.com/3M-Particulate-Filter-Organic-Filters/dp/B079X5C3QP/ref=sr_1_2?keywords=p100+air+filter&qid=1584469511&sr=8-2)
10. Test with a [simple inflator pump](https://www.amazon.com/gp/product/B013UQ0T2Y/ref=ppx_yo_dt_b_asin_title_o01_s00?ie=UTF8&psc=1) and achieve the same pressure control.
11. Develop a low-cost simple minute volume sensor/approximator.  Here is some information about the [downsides of being over/under on target volume.](https://www.acepnow.com/article/avoid-airway-catastrophes-extremes-minute-ventilation/).  Critical care seems to want up to 10-12L/Min.

To provide comments/feedback/offer help, [create a new issue](https://github.com/jcl5m1/ventilator/issues) on the project.  Thanks!


# Useful References/Docs about the supply shortages, and guidance around unapproved devices:

There is a growing list of useful documents around guidance for use of unapproved equipment, and information around estimated shortages of both ventilators and personal protective equipment.  We have moved them to [References Wiki to enable better updates](https://github.com/jcl5m1/ventilator/wiki/Useful-References) make it easier to maintain.

# Materials
1. [Continuous Positive Airway Pressure (CPAP)](https://en.wikipedia.org/wiki/Continuous_positive_airway_pressure) devices are commonly used by individuals to treat [sleep apnea](https://en.wikipedia.org/wiki/Sleep_apnea). As a result, CPAP blower components are fairly abundant. Used ones are available on eBay and new ones on Alibaba. In many of these devices, the blower is a simple BLDC motor that can be driven with an Electronics Speed Controller (ESC) used by quadrotors and RC cars.  The one I used is pulled from a [Respironics REMStar](https://www.google.com/search?q=respironics+remstar&safe=off&rlz=1C1CHBF_enUS809US809&sxsrf=ALeKk00Uw98vXCAyCiiSglgt-2Ucnz5RCg:1584357572342&source=lnms&tbm=isch&sa=X&ved=2ahUKEwj2mq_y757oAhXOvJ4KHV22D1EQ_AUoAnoECAwQBA).  In volume, these blower components can be under $20.  
  * Alternative: test a [12v inflator pump](https://www.amazon.com/gp/product/B013UQ0T2Y/ref=ppx_yo_dt_b_asin_title_o01_s00?ie=UTF8&psc=1) which is even more abundant.  
  * Alternative: [print a blower](https://drmrehorst.blogspot.com/2018/04/the-mother-of-all-print-cooling-fans.html) [[discussion]](https://github.com/jcl5m1/ventilator/issues/8)

2.  [Arduino Compatible Nano/Clone](https://www.amazon.com/s?k=Arduino+nano) to control the motor speed, breathing cycles, and handle user input.   Download the [Arduino IDE](https://www.arduino.cc/en/main/software) to program the [microcontroller software](https://github.com/jcl5m1/ventilator/tree/master/ventilator_control).

3. Brushless DC Motor [Electronic Speed Controller](https://www.amazon.com/Hobbypower-Brushless-Controller-Multicopter-Helicopter/dp/B00XKX5TBE/ref=sr_1_17?keywords=12v+ESC&qid=1584357758&sr=8-17) capable of at least 10A. 

4. Button or swtich for input control.  The code cycles through 5 levels of pressure, and double click to swtich between CPAP and BiPAP/Fixed PEEP mode.

5. 12v DC power supply that can handle at least 5A.  The blower works pretty had, and the breathing cycles can create voltage drops which will reset the Arduino if the supply if not strong enough.

6. Tubing and face mask.  If you search for [CPAP face mask](https://www.google.com/search?q=cpap+mask), you'll find many options/suppliers.  The full face mask that covers both the mouth and nose and enabled pressure buildup in the lungs.  As an alternative, see below for draft 3D printable parts.

7. Optional [inline 12v battery backup](https://www.amazon.com/gp/product/B07H8F5HYJ/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1).

# Building
This is the full system, with mask, tubing, pump, 12v battery, and swtich for input.  However, I would strongly recommend running with a DC power supply since the battery will only last about 3-4 hours.  <strong>Wearing the mask with a dead battery could cause dangerous CO2 build up.</strong>  This [particular battery](https://www.amazon.com/gp/product/B07H8F5HYJ/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1) can charge and supply power at the same time, acting as an inline battery backup to wall power.

![Full System](https://github.com/jcl5m1/ventilator/blob/master/images/full%20system.jpg?raw=true)

Below is a close up of the extracted blower.  It simply an BLDC motor driving a blower fan.  A small ESC and Arduino attached (technically a Teensy2.0++, but the code has now been changed to an Arduino Nano).  It has a 3d printed tapered hose adpater on it to fit in the hose.  The [microcontroller software](https://github.com/jcl5m1/ventilator/tree/master/ventilator_control) can be programmed with the [Arduino IDE](https://www.arduino.cc/en/main/software). The software goes through a short initialization sequence to configure the ESC, and then starts at the lowest CPAP setting.  You can use the switch to cycle through pressure levels, and double click to cycle through operating modes.

![Extracted CPAP Blower](https://github.com/jcl5m1/ventilator/blob/master/images/IMG_20200315_230153.jpg?raw=true)

Here is the schematic for the electronics. As you can see, it's very simple with only a few parts.  A slightly modifed version that is able to [sense the blower current is here](https://github.com/jcl5m1/ventilator/wiki/Breathing-Back-Pressure-Data-Analysis).
![schematic](https://github.com/jcl5m1/ventilator/blob/master/images/schematic_20200317.1.jpg?raw=true)


## Face mask / Tube Adapter
Since I have a CPAP, I happen to have a [Resmed AitFit F20](https://www.amazon.com/Resmed-AirFit-Frame-System-Cushion/dp/B07C9MR5RS) mask that I can use.  This is quite comfortable and covers both the nose and mouth to make a good seal.  However if you can't get a mask,  below is a prototype mouthguard that fit in the category of not-so-great-but-usable-if-you-have-a-lot-of-tape.  I am using some [elastic straps](https://www.amazon.com/gp/product/B07WZRZDPF/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1) as head gear.  However in order to get a good air seal to get enough positive pressure, this would require some tape.  A little bit of leakage is actually desirable to allow CO2 to escape near the mouth nose and to flush out the air over time; this prevents CO2 from being stuck in the tube.  <strong>WARNING: wearing this sealed to the mouth with tape without power may cause dangerous CO2 build up.</strong>

Download the [[3D model]](https://github.com/jcl5m1/ventilator/blob/master/3dmodels/mouthgaurd_v20200316.2.stl) [[Fusion 360 link]](https://a360.co/33rwb1R).

![Mouth Guard](https://github.com/jcl5m1/ventilator/blob/master/images/mouthguard.jpg?raw=true)
![Mouth Guard model](https://raw.githubusercontent.com/jcl5m1/ventilator/67c2725fc48a3bc147430cb8c9974f961ff96b7c/images/mouth_guard_model.JPG)


# Pressure Testing

This is a picture of my test rig to measure the output pressure.  It's a U-shaped tube with attached measuring tape to check displacement of water.  The actual liquid displacement is 2x the measured line since liquid is being push downwards on the other side of the U an equal amount, doubling the displacement. (I think this is the correct way to measure).  At full blast at 12v, it is achieving 45cmH20. <strong>[WARNING: Going above 20cmH20 can be DANGEROUS](https://github.com/jcl5m1/ventilator/wiki/Notes-from-chatting-with-a-pulmonologist)</strong>.  So, these blowers are plently powerful.

![Pressure measurement fixture](https://github.com/jcl5m1/ventilator/blob/master/images/test_fixture.jpg?raw=true)

Here are some images showing the dislacement of the liquid at differen speeds.  I happen to have Diet Dr. Pepper on hand which makes it easier to see the liquid level rather than just plain water.

![3cm](https://github.com/jcl5m1/ventilator/blob/master/images/3cm.jpg?raw=true) ![6cm](https://github.com/jcl5m1/ventilator/blob/master/images/6cm.jpg?raw=true) ![11cm](https://github.com/jcl5m1/ventilator/blob/master/images/11cm.jpg?raw=true) ![22.5cm](https://github.com/jcl5m1/ventilator/blob/master/images/22.5.jpg?raw=true)

Here's a [video of the blower pushing a breathing cycle of 16 cycles/min oscilating between 12 and 22cm/H20](https://photos.app.goo.gl/b3yMPE5QpdeduxKS6).

# Air Filtration - PAPR Option

An open air blower into someone's airway isn't great.  If possible, some filtration is preferred.  These parts were originally used for a DIY [Powered Air Purifying Respriator (PAPR)](https://en.wikipedia.org/wiki/Powered_air-purifying_respirator). However, adding a filter will definitely affect the output pressure.  It will depend on the blower you have if the motor can handle the additional load.  Given the headroom of the blower I tested, these CPAP blowers can probably handle the filter and still hit the target pressure.  However, I haven't done longterm thermal testing at high pressure and a strong filter. These adapter plates can be attached with liberal amounts of hot glue, and wrapping the unit in vinyl tape to get a reasonably airtight seal.  My blower's enclosure was not air-tight, so it is difficult to guarantee that 100% of the air is coming only through the filter.  Setting the blower to a low CPAP pressure, and leaving the face mask vent holes open will allow it to function reasonably as a PAPR.

* This is an adapter plate for a [3M P100 or N95 face mask filter cartridge](https://www.amazon.com/3M-50051138464658-Cartridge-Filter-Organic/dp/B07571LR2K/ref=sr_1_2?crid=36AKE548EW15U&keywords=3m+p100+cartridge&qid=1584418326&sprefix=3m+p100+cartrige%2Caps%2C202&sr=8-2). Download [[3D model]](https://github.com/jcl5m1/ventilator/blob/master/3dmodels/3M_filter_adapter.stl) [[Fusion 360 link]](https://a360.co/2QnQQOU)


![3M Apart](https://github.com/jcl5m1/ventilator/blob/master/images/3m_apart.jpg?raw=true) ![3M attached](https://github.com/jcl5m1/ventilator/blob/master/images/3m_attached.jpg?raw=true)

* This is an adapter plate for a [NATO 40mm gas mask filter cartridge](https://www.amazon.com/MIRA-Cartridge-Respiratory-Protection-Filter/dp/B07L38TYSF/ref=sr_1_2?keywords=40mm+gas+mask+filters&qid=1584418565&sr=8-2).  These fitlers are available with [CBRN](https://en.wikipedia.org/wiki/CBRN_defense) ratings which are needed for viral and bacterial defense similar to those [approved by the CDC/NIOSH](https://www.cdc.gov/niosh/npptl/standardsdev/cbrn/papr/default.html). Download [[3D model]](https://github.com/jcl5m1/ventilator/blob/master/3dmodels/NATO_40mm_adapter.stl) [[Fusion 360 link]](https://a360.co/3914gXs)



![NATO Apart](https://github.com/jcl5m1/ventilator/blob/master/images/NATO_apart.jpg?raw=true) ![NATO attached](https://github.com/jcl5m1/ventilator/blob/master/images/NATO_attached.jpg?raw=true)

* This is an option for an [inline filter](https://www.directhomemedical.com/cart/merchant.mvc?Screen=PROD&Product_Code=1-H1605-inline-bacterial-viral-cpap-filters&Store_Code=DHM&gclid=EAIaIQobChMIo-G35omi6AIVSrzACh29hwv3EAQYASABEgK9FPD_BwE) [[discussion]](https://github.com/jcl5m1/ventilator/issues/4) that looks easy to install, and would be helpful for non-CPAP blowers. 

# Update Log

Click [here](https://github.com/jcl5m1/ventilator/wiki) to see updates.
