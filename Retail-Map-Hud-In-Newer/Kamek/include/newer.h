#ifndef NEWER_H
#define NEWER_H 

extern int lastLevelIDs[];

void WriteAsciiToTextBox(nw4r::lyt::TextBox *tb, const char *source);

int getUnspentStarCoinCount();
int getStarCoinCount();
void getNewerLevelNumberString(int world, int level, wchar_t *dest);

//added
void getWorldString(int world, wchar_t *dest);
void getLevelString(int level, wchar_t *dest);
void getWorldNameAndLevelNumString(int level, wchar_t *dest);

#endif /* NEWER_H */
