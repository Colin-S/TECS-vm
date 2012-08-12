#ifndef CODEWRITER_H
#define CODEWRITER_H

int translate(Command_t* currentCommand);
int writeAdd(Command_t* currentCommand);
int writeSub(Command_t* currentCommand);
int writePush(Command_t* currentCommand);

#endif
