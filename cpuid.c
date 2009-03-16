
main ()
{
  char vendor_string[13];
  char features[12];
  long fms;
  int family, model, stepping;
  char *modelstr;

  cpuid (vendor_string, 0);
  vendor_string[12] = 0;

  fms = cpuid (features, 1);

  family = ((fms >> 8) & 15) + ((fms >> 20) & 0xff);
  model = ((fms >> 4) & 15) + ((fms >> 12) & 0xf0);
  stepping = fms & 15;
  
  #if BIT64
  modelstr = "x86_64";
  #else
  modelstr = "i486";// shouldn't we make this x86
  #endif

  if (strcmp (vendor_string, "GenuineIntel") == 0)
    {
      switch (family)
	{
	#if BIT32
	case 5:
	  if (model <= 2) { modelstr = "pentium";break;}
	  if (model >= 4) { modelstr = "pentiummmx";break;}
	  break;
        #endif
	case 6:
	  #if BIT32
	  if (model == 1) { modelstr = "pentiumpro";break;}
	  if (model <= 6) { modelstr = "pentium2";break;}
	  if (model <= 13){ modelstr = "pentium3";break;}
	  if (model == 14){ modelstr = "core";break;}
	  #endif
	  if (model == 15){ modelstr = "core2";break;}
	  if (model == 22){ modelstr = "core2";break;}
	  if (model == 23){ modelstr = "penryn";break;}
	  if (model == 26){ modelstr = "nehalem";break;}
	  if (model == 28){ modelstr = "atom";break;}
	  if (model == 29){ modelstr = "penryn";break;}
	  // modelstr = "core2";   // could fall back to core2
      	  break;
        case 15:
        #if BIT64
          cpuid(features,0x80000001);
          if ( features[8]&1 ){ modelstr = "netburstlahf";break;}
          modelstr = "netburst"
        #else
	  if (model <= 6) { modelstr = "pentium4";break;}
	  int feat = ((int *)features)[2];
          if (feat & 1) { modelstr = "prescott";break;}
        #endif
          break;
	}
    }
  else if (strcmp (vendor_string, "AuthenticAMD") == 0)
    {
      switch (family)
	{
	#if BIT32
	case 5:
	  if (model <= 3) { modelstr = "k5";break;}
	  if (model <= 7) { modelstr = "k6";break;}
	  if (model <= 8) { modelstr = "k62";break;}
	  if (model <= 9) { modelstr = "k63";break;}
	  break;
	case 6:
	  modelstr = "athlon";break;
        #endif
	case 15:
	  modelstr = "k8";break;
	case 16:
	  if (model == 2) { modelstr = "k10";break;} // phenom
	  if (model == 4) { modelstr = "k10";break;} //phenom II
	  break;
	}
    }
  #if BIT32
  else if (strcmp (vendor_string, "CyrixInstead") == 0)
    {
      /* Should recognize Cyrix' processors too.  */
    }
  else if (strcmp (vendor_string, "CentaurHauls") == 0)
    {
      switch (family)
	{
	case 6:
	  if (model < 9) { modelstr = "viac3";break;}
	  modelstr = "viac32";break;
	}
    }
  #endif
  printf ("%s\n", modelstr);
  return 0;
}
