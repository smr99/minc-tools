/* ----------------------------- MNI Header -----------------------------------
@NAME       : dump_acr_nema.c
@DESCRIPTION: Program to dump the contents of an acr-nema file.
@METHOD     : 
@GLOBALS    : 
@CREATED    : November 24, 1993 (Peter Neelin)
@MODIFIED   : $Log: extract_acr_nema.c,v $
@MODIFIED   : Revision 1.1  1993-11-24 11:25:23  neelin
@MODIFIED   : Initial revision
@MODIFIED   :
@COPYRIGHT  :
              Copyright 1993 Peter Neelin, McConnell Brain Imaging Centre, 
              Montreal Neurological Institute, McGill University.
              Permission to use, copy, modify, and distribute this
              software and its documentation for any purpose and without
              fee is hereby granted, provided that the above copyright
              notice appear in all copies.  The author and McGill University
              make no representations about the suitability of this
              software for any purpose.  It is provided "as is" without
              express or implied warranty.
---------------------------------------------------------------------------- */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <minc_def.h>
#include <acr_nema.h>

int main(int argc, char *argv[])
{
   char *pname, *filename;
   FILE *fp;
   Acr_File *afp;
   Acr_Group group_list;
   Acr_Element element;
   long element_length;
   char *string, *ptr;
   Acr_Element_Id element_id;

   /* Check arguments */
   pname = argv[0];
   if ((argc > 4) || (argc < 3)) {
      (void) fprintf(stderr, "Usage: %s [<file>] <group id> <element id>\n", 
                     pname);
      exit(EXIT_FAILURE);
   }

   /* Get file name */
   if (argc == 4)
      filename = argv[1];
   else 
      filename = NULL;

   /* Get element id (group and element) */
   element_id = MALLOC(sizeof(*element_id));
   string = argv[argc-2];
   element_id->group_id = strtol(string, &ptr, 0);
   if (ptr == string) {
      (void) fprintf(stderr, "%s: Error in group id (%s)\n", pname, string);
      exit(EXIT_FAILURE);
   }
   string = argv[argc-1];
   element_id->element_id = strtol(string, &ptr, 0);
   if (ptr == string) {
      (void) fprintf(stderr, "%s: Error in element id (%s)\n", pname, string);
      exit(EXIT_FAILURE);
   }

   /* Open input file */
   if (filename != NULL) {
      fp = fopen(filename, "r");
      if (fp == NULL) {
         (void) fprintf(stderr, "%s: Error opening file %s\n",
                        pname, filename);
         exit(EXIT_FAILURE);
      }
   }
   else {
      fp = stdin;
   }

   /* Connect to input stream */
   afp=acr_file_initialize(fp, 0, acr_stdio_read);

   /* Read in group list up to group */
   (void) acr_input_group_list(afp, &group_list, element_id->group_id);

   /* Look for element */
   element = acr_find_group_element(group_list, element_id);

   /* Print out value of element */
   if (element != NULL) {
      element_length = acr_get_element_length(element);
      if (element_length > 0) {
         (void) fwrite(acr_get_element_data(element), sizeof(char),
                       (size_t) element_length, stdout);
      }
   }

   exit(EXIT_SUCCESS);

}