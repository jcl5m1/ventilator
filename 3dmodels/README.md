# 3D models 

## Requirements & recommendations

**Supported file formats**

- STL - for 3D printed models
- STEP - for objects representing part to be used as a reference
- Application specific projects (like SolidWorks, Fusion360) - CAD formats other users could edit 

**Unsupported file formats**
- Mesh-based objects (.obj, .3dmax, etc) - these formats are not editable by CAD software and should be avoided.

**3D printing recommendations:**

1. Part dimensions should be within 200x200x200mm - print area limits for most 3D printers
2. Avoid overhangs - parts should be printable without Support
3. For bigger parts please also try to provide stacked version - i.e. parts on top of each other, so we could print them in bulk
4. Recommended print settings should be provided for printing with PET-G (material accepted for medical purposes)


# Reference sheet on original parts

## CPAP filter

### Description

A filter used by ventilators for filtering out dust and moisture particles. Original part consists of two pieces melted together, and filtering membrane inside. The membrane is just a piece of cloth and fiber, it does not have any complex structures HEPA-filters and such.

Accurate part dimensions are captured in **_Inline_filter_VB-65mm.STEP_** file

Currently development is happening in https://github.com/jcl5m1/ventilator/tree/cpap-ventilator branch

### Requirements
* Part should be completely sealable (possibly with the help of superglue of medical silicone)
* Part should contain special filter inside

**Reference photos:**

![CPAP filter front view](images/CPAP-filter-front.jpg)
![Membrane](images/CPAP-filter-membrane.jpg)
![CPAP filter cut open 1](images/CPAP-filter-open.jpg)
![CPAP filter cut open 2](images/CPAP-filter-open-2.jpg)
![Membrane dimensions](images/CPAP-membrane.jpg)

### Solution #1
The 3D-printed part that consists of 2 pieces. Parts have very small tollerances and are connected using mechanical lock to make it re-usable. Requires additional sealant for complete air-tightness.
![3D printed CPAP filter](images/CPAP-3D-printed-filter.jpg)
![3D printed CPAP filter connected to tubes](images/CPAP-3D-printed-filter-connected.jpg)
![Membrane location inside 3D printed filter](images/CPAP-3D-printed-filter-opened.jpg)


**Recommended print settings**
- Infill rate: 70% or higher
- Lower part to be printed from inlet upwards. Recommended to use 3mm brim for stability
- Because of steep anges use higher print temperature to improve filament adhersion 

### Solution #2
*_Design in progress_*
Monolitic part that has filter inserted during print process. Does not require extra sealant. Not reusable. Requires insertion of custom G-code after particular layer in order to manually insert filter in the model


