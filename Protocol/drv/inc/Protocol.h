#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_
#include "kernel.h"

#define MAXSIZE	200

typedef U8 QueueDataType;
typedef U16 QueueLengthType;

typedef struct _tagQueueInfo
{
	QueueDataType queuedata[MAXSIZE];
	QueueLengthType Front;
	QueueLengthType Rear;
}CQueueInfo, *PQueueInfo;

VOID Init(PQueueInfo pQueueInfo);
BOOL AddQueue(PQueueInfo pQueueInfo, QueueDataType qValue);
BOOL DeleteQueue(PQueueInfo pQueueInfo, QueueDataType *pqValue);
QueueLengthType GetQueueLength(PQueueInfo pQueueInfo);





#endif

