
//Pressure sensor tube adapter
//Version 1.0
//March 20, 2020
/* Designed to fit 
//     * over the top of an MEAS MS5611 pressure sensor
//     * connect to a 3-7mm ID flexible tubing 
 MEAS MS5611 specifications are available at https://www.te.com/commerce/DocumentDelivery/DDEController?Action=showdoc&DocId=Data+Sheet%7FMS5611-01BA03%7FB3%7Fpdf%7FEnglish%7FENG_DS_MS5611-01BA03_B3.pdf%7FCAT-BLPS0036 
*/

/*

Copyright 2020 Alex Bobotek

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/


//MS5611 dimensions
MSwidth=3;
MSlen=5;
MSh=1.8;

MSgap=0.15;
MShoodThick=.8;
MSheadroom=1;       // free room over sensor inside hood
MSceilingThick=2.5;

barbID=2;
barbLen=15;
barbMinOD=3;
barbMaxOD=7.5;
barbMaxTaper=4;
barbMinTaper=3.5;
barbTaperLen=MSceilingThick+1.5;
taperSlope=1/3; // ratio of diameter change to height change
taperZ=3;
neckZ=1;


translate([10,10,0]) block();
barb();

module block() {
    difference() {
        cube([MSwidth+2*MSgap+2*MShoodThick,MSlen+2*MSgap+2*MShoodThick,MSh+MSheadroom+MSceilingThick]); // Outer shell
        translate([MShoodThick,MShoodThick,0]) cube([MSwidth+2*MSgap,MSlen+2*MSgap,MSh+MSheadroom]);    // Inner cavity
        translate([MSwidth/2+MSgap+MShoodThick,MSlen/2+MSgap+MShoodThick,MSh+MSheadroom]) 
            cylinder(d1=barbMinTaper,d2=barbMaxTaper,h=MSceilingThick, $fn=72);    // Tapered socket
        //translate([MSwidth/2+MSgap+MShoodThick,MSlen/2+MSgap+MShoodThick,0])
            //cylinder(d=barbID,h=MSh+MSheadroom+MSceilingThick, $fn=24);
    }
};
    


module  ring(minD,ringH) {
    cylinder(d1=minD,d2=minD+ringH*taperSlope,h=ringH,$fn=72);
    translate([0,0,ringH]) cylinder(d=minD,h=neckZ,$fn=72);
}

module barb(){
    difference() {
        union() {
            translate([0,0,3*(taperZ+neckZ)]) cylinder(d2=barbMinTaper,d1=barbMaxTaper,h=MSceilingThick, $fn=72);    // Tapered socket
            translate([0,0,2*(taperZ+neckZ)]) ring(minD=4,ringH=3);
            translate([0,0,1*(taperZ+neckZ)]) ring(minD=3.5,ringH=3);
            translate([0,0,0*(taperZ+neckZ)]) ring(minD=3,ringH=3);
        }
        cylinder(d=barbID,h=4*(taperZ+neckZ),$fn=72);
    }
}