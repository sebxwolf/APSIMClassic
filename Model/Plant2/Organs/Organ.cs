using System;
using System.Collections.Generic;
using System.Text;

public interface AboveGround
   {
   }
public interface BelowGround
   {
   }
public interface Reproductive
   {
   }
abstract public class Organ : Instance
   {
   public Plant Plant
      {
      get { return (Plant)Root; }
      }

   abstract public Biomass Live { get;}
   abstract public Biomass Dead { get;}

   abstract public double DMDemand{ get; }
   abstract public double DMSupply { get; }
   abstract public double DMRetranslocationSupply { get;}
   abstract public double DMAllocation {set; }
   abstract public double DMRetranslocation { set;}

   abstract public double NDemand { get; }
   abstract public double NUptakeSupply { get; }
   abstract public double NRetranslocationSupply { get; }
   abstract public double NAllocation { set; }
   abstract public double NRetranslocation { set; }
   abstract public double NUptake { set; }

   abstract public double WaterDemand { get; }
   abstract public double WaterSupply { get; }
   abstract public double WaterAllocation { get; set;}
   virtual public void DoWaterUptake(double Demand) { }
   virtual public void DoPotentialGrowth() { }
   virtual public void DoActualGrowth() { }
   }
   
