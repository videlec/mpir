#define CPUMODELSTR(x)  modelstr="x"
#define CPUTRACE(x) 	TRACE(printf("  x"))
#define CPUCPUVEC(x)	CPUVEC_SETUP_x
#define CPUMODEL(x)	do{CPUMODELSTR(x);CPUTRACE(x);CPUCPUVEC(x);}while(0)

      char vendor_string[13];
      char dummy_string[12];
      long fms;
      int family, model,stepping;
      char *modelstr;

      __gmpn_cpuid (vendor_string, 0);
      vendor_string[12] = 0;
      fms = __gmpn_cpuid (dummy_string, 1);
      family = (fms >> 8) & 15;
      model = (fms >> 4) & 15;
      family += ((fms >> 20) & 0xff);
      model +=  ((fms >> 12) & 0xf0);
      stepping = fms & 15;
      modelstr= "i486";
      if (strcmp (vendor_string, "GenuineIntel") == 0)
        {
          switch (family)
            {           
            case 4:
              CPUMODEL(i486);
              break;
            case 5:
              CPUMODEL(pentium);
              if (model >= 4)
                {CPUMODEL(pentiummmx);CPUVEC_SETUP_pentium_mmx;
                }
              break;
            case 6:
              TRACE (printf ("  pentiumpro\n"));              
              #if BIT32
              CPUVEC_SETUP_p6;
              #endif
              if (model >= 2)
                {
                  TRACE (printf ("  pentium2\n"));
                  #if BIT32
                  CPUVEC_SETUP_p6_mmx;
                  #endif
                }
              if (model >= 7)
                {
                  TRACE (printf ("  pentium3\n"));
                  #if BIT32
                  CPUVEC_SETUP_p6_p3mmx;
                  #endif
                }
              if( model==14) modelstr="core";
              if( model<=13) modelstr="pentium3";
              if( model<=6) modelstr="pentium2";
              if( model<=1) modelstr="pentiumpro";
              #if BIT64
              modelstr="core2";
              if (model==15 || model==22)
                 {TRACE (printf ("  core2\n"));modelstr="core2";CPUVEC_SETUP_core2;break;}
                 if (model==23 || model==29)
                 {TRACE (printf ("  penryn\n"));modelstr="penryn";CPUVEC_SETUP_core2_penryn;break;}
                 if (model==26)
                 {TRACE (printf ("  nehalem\n"));modelstr="nehalem";CPUVEC_SETUP_nehalem;break;}
                 if (model==28)
                 {TRACE (printf ("  atom\n"));modelstr="atom";CPUVEC_SETUP_atom;break;}                 
              #endif
              break;
            #if BIT32
            case 15:
               TRACE (printf ("  pentium4\n"));
               CPUVEC_SETUP_pentium4;
               CPUVEC_SETUP_pentium4_mmx;
               CPUVEC_SETUP_pentium4_sse2;
               if (model <= 6) modelstr = "pentium4";
               else modelstr = "p4???";
               int feat = ((int *)dummy_string)[2];
               if (feat & 1) modelstr = "prescott";
               break;
            #endif              
            #if BIT64
            case 15:
              TRACE (printf ("  netburst\n"));
              __gmpn_cpuid (dummy_string, 0x80000001);
              if( (dummy_string[8] & 1) )
                {CPUVEC_SETUP_netburst_netburstlahf;modelstr="netburstlahf"; break;}
              else
                {CPUVEC_SETUP_netburst;modelstr="netburst"; break;}
            #endif
}
        }
      else if (strcmp (vendor_string, "AuthenticAMD") == 0)
        {
          switch (family)
            {
            case 5:
              if (model <= 3)
                {modelstr="k5";
                  TRACE (printf ("  k5\n"));
                }
              else
                {modelstr="k6";
                  TRACE (printf ("  k6\n"));
                  #if BIT32
                   CPUVEC_SETUP_k6;
                   CPUVEC_SETUP_k6_mmx;
                   #endif
                  if (model >= 8)
                    {modelstr="k62";
                      TRACE (printf ("  k62\n"));
                      #if BIT32
                      CPUVEC_SETUP_k6_k62mmx;
                      #endif
                    }
                  if (model >= 9)
                    { modelstr="k63";
                      TRACE (printf ("  k63\n"));
                    }
                }
              break;
            case 6:
              TRACE (printf ("  athlon\n"));
            athlon:
                modelstr="athlon";
               #if BIT32
               CPUVEC_SETUP_k7;
               CPUVEC_SETUP_k7_mmx;
               #endif
              break;
             case 15:
              TRACE (printf ("  k8\n"));
               #if BIT32
               modelstr="athlon";
               CPUVEC_SETUP_k7;
               CPUVEC_SETUP_k7_mmx;
               break;
               #endif
              #if BIT64
              modelstr="k8";
              CPUVEC_SETUP_k8;
              break;
              #endif
              #if BIT64
              case 16:
              modelstr="k10";
              TRACE (printf ("  k10\n"));
              CPUVEC_SETUP_k8_k10;
              break;
              #endif
            }
        }
      else if (strcmp (vendor_string, "CentaurHauls") == 0)
        {
          switch (family)
            {
            case 6:
              TRACE (printf ("  viac3\n"));modelstr="viac3";
              if (model >= 9)
                {modelstr="viac3c";
                  TRACE (printf ("  viac32\n"));
                }
              break;
            }
        }
      else if (strcmp (vendor_string, "CyrixInstead") == 0)
        {
          /* Should recognize Cyrix' processors too.  */
          TRACE (printf ("  cyrix something\n"));
        }
