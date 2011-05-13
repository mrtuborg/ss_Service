#include "ServicesSpecifier.h"

int services_specifier::load_specs(const char *fileName)
{
    XMLNode xMainNode     = XMLNode::openFileHelper(fileName,"plist");
    XMLNode xFunctions    = xMainNode.getChildNodeByPath("dict/dict");
    XMLNode xCurrFunction;
    XMLNode xInputVars, xOutputVars;
    XMLNode xVar;
    int inputVars_count  = 0;
    int outputVars_count = 0;
    char *curr_dict_title = 0;

    int func_count = xMainNode.getChildNodeByPath("dict/dict").nChildNode("dict");
    printf("service: %s\n",   xMainNode.getChildNode("dict").getChildNode("string",0).getText());
    printf("sevice_id: %s\n", xMainNode.getChildNode("dict").getChildNode("string",1).getText());
    printf("functions count: %d\n", func_count);

    for (int i = 0; i < func_count; i++)
    {
        xCurrFunction = xFunctions.getChildNode("dict", i);

        printf("id = %s, title: %s\n", xFunctions.getChildNode("key", i).getText(), xCurrFunction.getChildNode("string").getText());

        int descr_count = xCurrFunction.nChildNode("dict");
        // printf("descr_count = %d\n", descr_count);
        inputVars_count  = 0;
        outputVars_count = 0;

        for (int j = 0; j < descr_count; j++)
        {
            // input
            curr_dict_title = (char*) xCurrFunction.getChildNode("key",j).getText();
            if ((curr_dict_title) && (strcmp(curr_dict_title, "input") == 0))
            {
                xInputVars = xCurrFunction.getChildNode("dict", j);
                inputVars_count = xInputVars.nChildNode("key");
            }

            // output
            if ((curr_dict_title) && (strcmp(curr_dict_title, "output") == 0))
            {
                xOutputVars = xCurrFunction.getChildNode("dict", j);
                outputVars_count = xOutputVars.nChildNode("key");
            }
        }


    //printf("function input variables count = %d\n", inputVars_count);
    //printf("function output variables count = %d\n", outputVars_count);

    int id = 0;
    int xVar_descr_count = 0;
    int user_types_count = 0;

      for (int k = 0; i < inputVars_count; i++)
      {
          id = atoi( xInputVars.getChildNode("key", i).getText() );
          printf("\t\t");
          printf("id = %d ", id);
          xVar = xInputVars.getChildNode("dict", i);
          xVar_descr_count = xVar.nChildNode("key");

         for (int j = 0; j < xVar_descr_count; j++)
         {
              //xInputVars.getChildNode("key").getText(); // type

          curr_dict_title = (char*) xVar.getChildNode("key", j).getText();
          if ((curr_dict_title) && (strcmp(curr_dict_title, "type") == 0))
          {
              printf("type = %s ", strOrtsTypes[atoi (xVar.getChildNode("integer").getText())]);
          }

          if ((curr_dict_title) && (strcmp(curr_dict_title, "typedef") == 0))
          {
              printf("type_USER ");
              xVar = xVar.getChildNode("dict", user_types_count++);
              outputVars_count = xVar.nChildNode("key");
              printf("vars = %d\n", outputVars_count);
              for (int m = 0; m < outputVars_count; m++)
              {
                  xVar = xVar.getChildNode("dict", m);
                  printf("id = %d ", m);
                  for (int z = 0; z < 2; z++){
                    curr_dict_title = (char*) xVar.getChildNode("key", z).getText();
                    //printf("--%s--\n", curr_dict_title);
                    if ((curr_dict_title) && (strcmp(curr_dict_title, "type") == 0))
                    {
                        printf(" type = %s ", strOrtsTypes[atoi (xVar.getChildNode("integer").getText())]);
                    }

                    if ((curr_dict_title) && (strcmp(curr_dict_title, "title") == 0))
                    {
                        printf(" title = %s ", xVar.getChildNode("string").getText());
                    }



                  }
                  printf("\n");

                  xVar = xVar.getParentNode();
              }

          }

          if ((curr_dict_title) && (strcmp(curr_dict_title, "title") == 0))
          {
              printf("title = %s ", xVar.getChildNode("string").getText());
          }




         }
         printf("\n");
      }
    }
}

int services_specifier::install_specs()
{

}

int services_specifier::dbgPrint()
{

}

services_specifier::services_specifier()
{

}

services_specifier::~services_specifier()
{

}
