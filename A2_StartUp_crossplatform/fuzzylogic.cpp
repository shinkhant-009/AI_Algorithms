#include <algorithm>
#include "fuzzylogic.h"

/////////////////////////////////////////////////////////////////

// Initialise Fuzzy Rules

void initFuzzyRules(fuzzy_system_rec *fl)
{
   int r = 0;
   int x, y;

   // Fuzzy Association Matrix (FAM).
   const short fam[5][5] = {
       {out_nvl, out_nl, out_nl, out_ns, out_ze},
       {out_nl, out_nl, out_ns, out_ze, out_ps},
       {out_nl, out_ns, out_ze, out_ps, out_pl},
       {out_ns, out_ze, out_ps, out_pl, out_pl},
       {out_ze, out_ps, out_pl, out_pl, out_pvl}};

   // Generate one rule for every combination of X and Y fuzzy sets.
   for (x = in_nl; x <= in_pl; x++)
   {
      for (y = in_nl; y <= in_pl; y++)
      {
         fl->rules[r].inp_index[0] = INPUT_X;
         fl->rules[r].inp_index[1] = INPUT_Y;
         fl->rules[r].inp_fuzzy_set[0] = x;
         fl->rules[r].inp_fuzzy_set[1] = y;
         fl->rules[r].out_fuzzy_set = fam[x][y];
         r++;
      }
   }

   return;
}

// Define the five membership functions (NL, NS, ZE, PS and PL).
// X and Y use identical membership-function parameters.
void initMembershipFunctions(fuzzy_system_rec *fl)
{

   // /* The X membership functions */
   // fl->inp_mem_fns[INPUT_X][in_nl] = init_trapz(-3.0, -1.5, 0, 0, left_trapezoid);
   // fl->inp_mem_fns[INPUT_X][in_ns] = init_trapz(-3.0, -1.5, -1.5, 0.0, regular_trapezoid);
   // fl->inp_mem_fns[INPUT_X][in_ze] = init_trapz(-1.5, 0.0, 0.0, 1.5, regular_trapezoid);
   // fl->inp_mem_fns[INPUT_X][in_ps] = init_trapz(0.0, 1.5, 1.5, 3.0, regular_trapezoid);
   // fl->inp_mem_fns[INPUT_X][in_pl] = init_trapz(1.5, 3.0, 0, 0, right_trapezoid);

   // /* The Y dot membership functions */
   // fl->inp_mem_fns[INPUT_Y][in_nl] = init_trapz(-3.0, -1.5, 0, 0, left_trapezoid);
   // fl->inp_mem_fns[INPUT_Y][in_ns] = init_trapz(-3.0, -1.5, -1.5, 0.0, regular_trapezoid);
   // fl->inp_mem_fns[INPUT_Y][in_ze] = init_trapz(-1.5, 0.0, 0.0, 1.5, regular_trapezoid);
   // fl->inp_mem_fns[INPUT_Y][in_ps] = init_trapz(0.0, 1.5, 1.5, 3.0, regular_trapezoid);
   // fl->inp_mem_fns[INPUT_Y][in_pl] = init_trapz(1.5, 3.0, 0, 0, right_trapezoid);

   // /* The X membership functions */
   // fl->inp_mem_fns[INPUT_X][in_nl] = init_trapz(-2.0, -2.0, -2.0, -1.0, left_trapezoid);
   // fl->inp_mem_fns[INPUT_X][in_ns] = init_trapz(-2.0, -1.0, -0.5, 0.0, regular_trapezoid);
   // fl->inp_mem_fns[INPUT_X][in_ze] = init_trapz(-1.0, -0.25, 0.25, 1.0, regular_trapezoid);
   // fl->inp_mem_fns[INPUT_X][in_ps] = init_trapz(0.0, 0.5, 1.0, 2.0, regular_trapezoid);
   // fl->inp_mem_fns[INPUT_X][in_pl] = init_trapz(1.0, 2.0, 2.0, 2.0, right_trapezoid);

   // /* The Y dot membership functions */
   // fl->inp_mem_fns[INPUT_Y][in_nl] = init_trapz(-2.0, -2.0, -2.0, -1.0, left_trapezoid);
   // fl->inp_mem_fns[INPUT_Y][in_ns] = init_trapz(-2.0, -1.0, -0.5, 0.0, regular_trapezoid);
   // fl->inp_mem_fns[INPUT_Y][in_ze] = init_trapz(-1.0, -0.25, 0.25, 1.0, regular_trapezoid);
   // fl->inp_mem_fns[INPUT_Y][in_ps] = init_trapz(0.0, 0.5, 1.0, 2.0, regular_trapezoid);
   // fl->inp_mem_fns[INPUT_Y][in_pl] = init_trapz(1.0, 2.0, 2.0, 2.0, right_trapezoid);

   /* The X membership functions */
   fl->inp_mem_fns[INPUT_X][in_nl] = init_trapz(-2.0, -2.0, -2.0, -1.0, left_trapezoid);
   fl->inp_mem_fns[INPUT_X][in_ns] = init_trapz(-2.0, -1.0, -1.0, 0.0, regular_trapezoid);
   fl->inp_mem_fns[INPUT_X][in_ze] = init_trapz(-1.0, -0.25, 0.25, 1.0, regular_trapezoid);
   fl->inp_mem_fns[INPUT_X][in_ps] = init_trapz(0.0, 1.0, 1.0, 2.0, regular_trapezoid);
   fl->inp_mem_fns[INPUT_X][in_pl] = init_trapz(1.0, 2.0, 2.0, 2.0, right_trapezoid);

   /* The Y membership functions */
   fl->inp_mem_fns[INPUT_Y][in_nl] = init_trapz(-2.0, -2.0, -2.0, -1.0, left_trapezoid);
   fl->inp_mem_fns[INPUT_Y][in_ns] = init_trapz(-2.0, -1.0, -1.0, 0.0, regular_trapezoid);
   fl->inp_mem_fns[INPUT_Y][in_ze] = init_trapz(-1.0, -0.25, 0.25, 1.0, regular_trapezoid);
   fl->inp_mem_fns[INPUT_Y][in_ps] = init_trapz(0.0, 1.0, 1.0, 2.0, regular_trapezoid);
   fl->inp_mem_fns[INPUT_Y][in_pl] = init_trapz(1.0, 2.0, 2.0, 2.0, right_trapezoid);

   return;
}

// Configure the fuzzy controller, output singleton values and rule storage.
void initFuzzySystem(fuzzy_system_rec *fl)
{
   fl->no_of_inputs = 2;
   fl->no_of_rules = 25; // Five X sets x five Y sets.
   fl->no_of_inp_regions = 5;
   fl->no_of_outputs = 9;

   // coefficient_A = 8.0; // pole angle      (rad)
   // coefficient_B = 1.0; // pole angular velocity (rad/s)
   // coefficient_C = 0.2; // cart position   (m)
   // coefficient_D = 0.2; // cart velocity   (m/s)

   // ****
   coefficient_A = 10.0; // pole angle      (rad)
   coefficient_B = 1.0;  // pole angular velocity (rad/s)
   coefficient_C = 0.60; // cart position   (m)
   coefficient_D = 0.10; // cart velocity   (m/s)

   // fl->output_values[out_nvl] = -75.0;
   // fl->output_values[out_nl] = -45.0;
   // fl->output_values[out_nm] = -24.0;
   // fl->output_values[out_ns] = -14.0;
   // fl->output_values[out_ze] = 0.0;
   // fl->output_values[out_ps] = 14.0;
   // fl->output_values[out_pm] = 24.0;
   // fl->output_values[out_pl] = 45.0;
   // fl->output_values[out_pvl] = 75.0;

   // fl->output_values[out_nvl] = -60.0;
   // fl->output_values[out_nl] = -40.0;
   // fl->output_values[out_nm] = -26.0;
   // fl->output_values[out_ns] = -14.0;
   // fl->output_values[out_ze] = 0.0;
   // fl->output_values[out_ps] = 14.0;
   // fl->output_values[out_pm] = 26.0;
   // fl->output_values[out_pl] = 40.0;
   // fl->output_values[out_pvl] = 60.0;

   // ****
   fl->output_values[out_nvl] = -150.0;
   fl->output_values[out_nl] = -105.0;
   fl->output_values[out_nm] = -67.5;
   fl->output_values[out_ns] = -33.0;
   fl->output_values[out_ze] = 0.0;
   fl->output_values[out_ps] = 33.0;
   fl->output_values[out_pm] = 67.5;
   fl->output_values[out_pl] = 105.0;
   fl->output_values[out_pvl] = 150.0;


   // Allocate storage for the 25 fuzzy rules.
   fl->rules = (rule *)malloc((size_t)(fl->no_of_rules * sizeof(rule)));
   fl->allocated = true;
   initFuzzyRules(fl);
   initMembershipFunctions(fl);
   return;
}

// Create a trapezoidal membership function and calculate its slopes.
trapezoid init_trapz(float x1, float x2, float x3, float x4, trapz_type typ)
{
   trapezoid trz;
   trz.a = x1;
   trz.b = x2;
   trz.c = x3;
   trz.d = x4;
   trz.tp = typ;
   // switch (trz.tp)
   // {
   //    case regular_trapezoid:
   //       trz.l_slope = 1.0 / (trz.b - trz.a);
   //       trz.r_slope = 1.0 / (trz.c - trz.d);
   //       break;

   //    case left_trapezoid:
   //       trz.r_slope = 1.0 / (trz.a - trz.b);
   //       trz.l_slope = 0.0;
   //       break;

   //    case right_trapezoid:
   //       trz.l_slope = 1.0 / (trz.b - trz.a);
   //       trz.r_slope = 0.0;
   //       break;
   // } /* end switch  */

   switch (trz.tp)
   {
   // Regular Trapezoid
   case regular_trapezoid:
      trz.l_slope = 1.0 / (trz.b - trz.a);
      trz.r_slope = 1.0 / (trz.c - trz.d);
      break;

   // Left Trapezoid
   case left_trapezoid:
      trz.r_slope = 1.0 / (trz.c - trz.d);
      trz.l_slope = 0.0;
      break;

   // Right Trapezoid
   case right_trapezoid:
      trz.l_slope = 1.0 / (trz.b - trz.a);
      trz.r_slope = 0.0;
      break;
   }

   return trz;
} /* end function */

// Calculate the membership degree of x in the given fuzzy set.
float trapz(float x, trapezoid trz)
{
   // switch (trz.tp)
   // {

   // case left_trapezoid:
   //    if (x <= trz.a)
   //       return 1.0;
   //    if (x >= trz.b)
   //       return 0.0;
   //    /* a < x < b */
   //    return trz.r_slope * (x - trz.b);

   // case right_trapezoid:
   //    if (x <= trz.a)
   //       return 0.0;
   //    if (x >= trz.b)
   //       return 1.0;
   //    /* a < x < b */
   //    return trz.l_slope * (x - trz.a);

   // case regular_trapezoid:
   //    if ((x <= trz.a) || (x >= trz.d))
   //       return 0.0;
   //    if ((x >= trz.b) && (x <= trz.c))
   //       return 1.0;
   //    if ((x >= trz.a) && (x <= trz.b))
   //       return trz.l_slope * (x - trz.a);
   //    if ((x >= trz.c) && (x <= trz.d))
   //       return trz.r_slope * (x - trz.d);
   // } /* End switch  */

   switch (trz.tp)
   {
   // Left Trapezoid
   case left_trapezoid:
      if (x <= trz.c)
         return 1.0;
      if (x >= trz.d)
         return 0.0;
      /* a < x < b */
      return trz.r_slope * (x - trz.d);

   // Right Trapezoid
   case right_trapezoid:
      if (x <= trz.a)
         return 0.0;
      if (x >= trz.b)
         return 1.0;
      /* a < x < b */
      return trz.l_slope * (x - trz.a);

   // Regular Trapezoid
   case regular_trapezoid:
      if ((x <= trz.a) || (x >= trz.d))
         return 0.0;
      if ((x >= trz.b) && (x <= trz.c))
         return 1.0;
      if ((x >= trz.a) && (x <= trz.b))
         return trz.l_slope * (x - trz.a);
      if ((x >= trz.c) && (x <= trz.d))
         return trz.r_slope * (x - trz.d);
   } /* End switch  */

   return 0.0; /* should not get to this point */
} /* End function */

// Apply fuzzy AND by returning the minimum input membership degree.
float min_of(float values[], int no_of_inps)
{
   int i;
   float val;
   val = values[0];
   for (i = 1; i < no_of_inps; i++)
   {
      if (values[i] < val)
         val = values[i];
   }
   return val;
}

// Evaluate all rules and produce a crisp force using weighted-average
// defuzzification. A rule's firing strength is its minimum input membership.
float fuzzy_system(float inputs[], fuzzy_system_rec fz)
{
   int i, j;
   short variable_index, fuzzy_set;
   float sum1 = 0.0, sum2 = 0.0, weight;
   float m_values[MAX_NO_OF_INPUTS];

   for (i = 0; i < fz.no_of_rules; i++)
   {
      for (j = 0; j < fz.no_of_inputs; j++)
      {
         variable_index = fz.rules[i].inp_index[j];
         fuzzy_set = fz.rules[i].inp_fuzzy_set[j];
         m_values[j] = trapz(inputs[variable_index],
                             fz.inp_mem_fns[variable_index][fuzzy_set]);
      } /* end j  */

      weight = min_of(m_values, fz.no_of_inputs);

      sum1 += weight * fz.output_values[fz.rules[i].out_fuzzy_set];
      sum2 += weight;
   } /* end i  */

   if (fabs(sum2) < TOO_SMALL)
   {
      cout << "\r\nFLPRCS Error: Sum2 in fuzzy_system is 0.  Press key: " << endl;
      //~ getch();
      //~ exit(1);
      return 0.0;
   }

   return (sum1 / sum2);
} /* end fuzzy_system  */

// Release dynamically allocated fuzzy-rule storage.
void free_fuzzy_rules(fuzzy_system_rec *fz)
{
   if (fz->allocated)
   {
      free(fz->rules);
   }

   fz->allocated = false;
   return;
}
