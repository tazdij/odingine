#ifndef __VIKING_STRING_H__
#define __VIKING_STRING_H__

// The following function was found on http://xoomer.virgilio.it/acantato/dev/wildcard/wildmatch.html, where it was attributed to 
// the C/C++ Users Journal, written by Mike Cornelison. It is a little ugly, but it is FAST. Use this as an excercise in not reinventing the
// wheel, even if you see gotos. 

int WildcardMatch(const char *pat, const char *str) {
   int i, star;

new_segment:

   star = 0;
   if (*pat == '*') {
      star = 1;
      do { pat++; } while (*pat == '*'); /* enddo */
   } /* endif */

test_match:

   for (i = 0; pat[i] && (pat[i] != '*'); i++) {
      //if (mapCaseTable[str[i]] != mapCaseTable[pat[i]]) {
	  if (str[i] != pat[i]) {
         if (!str[i]) return 0;
         if ((pat[i] == '?') && (str[i] != '.')) continue;
         if (!star) return 0;
         str++;
         goto test_match;
      }
   }
   if (pat[i] == '*') {
      str += i;
      pat += i;
      goto new_segment;
   }
   if (!str[i]) return 1;
   if (i && pat[i - 1] == '*') return 1;
   if (!star) return 0;
   str++;
   goto test_match;
}


#endif