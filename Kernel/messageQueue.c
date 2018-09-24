#include <stdlib.h>
#include <stdint.h>
#include "messageQueue.h"
#include "buddy.h"
#include "process.h"


void initMessaageQueue(){
  tmailbox_list * mailboxes = mymalloc(sizeof(struct mailbox_list));
  mailboxes->head = NULL;

}
