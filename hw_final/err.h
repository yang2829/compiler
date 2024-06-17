/************************ err.h **********************/
#define ERRMAX 27
char errmsgs[ERRMAX][36]=
{
  "必須跟著句點.",              /*errmsgs[0] */
  "遺漏PROGRAM",                /*errmsgs[1] */
  "PROGRAM後必須跟著識別字",    /*errmsgs[2] */
  "PROGRAM後識別字必須跟著;",   /*errmsgs[3] */
  "遺漏CONST",                  /*errmsgs[4] */
  "遺漏等號=",                  /*errmsgs[5] */
  "遺漏逗號,或分號;",           /*errmsgs[6] */
  "遺漏VAR",                    /*errmsgs[7] */
  "遺漏:=",                     /*errmsgs[8] */
  "遺漏CALL",                   /*errmsgs[9] */
  "遺漏BEGIN",                  /*errmsgs[10]*/
  "遺漏END",                    /*errmsgs[11]*/
  "遺漏IF",                     /*errmsgs[12]*/
  "遺漏THEN",                   /*errmsgs[12]*/
  "遺漏WHILE",                  /*errmsgs[14]*/
  "WHILE敘述錯誤,遺漏DO",       /*errmsgs[15]*/
  "遺漏READ",                   /*errmsgs[16]*/
  "遺漏(",                      /*errmsgs[17]*/
  "遺漏)",                      /*errmsgs[18]*/
  "遺漏WRITE",                  /*errmsgs[19]*/
  "關係運算子錯誤",             /*errmsgs[20]*/
  "遺漏識別字",                 /*errmsgs[21]*/
  "遺漏數字",                   /*errmsgs[22]*/
  "飛越至下一個敘述",           /*errmsgs[23]*/
  "CONST宣告常數重複",          /*errmsgs[24]*/
  "VAR宣告變數重複",            /*errmsgs[25]*/
  "識別字沒有宣告"              /*errmsgs[26]*/
};
