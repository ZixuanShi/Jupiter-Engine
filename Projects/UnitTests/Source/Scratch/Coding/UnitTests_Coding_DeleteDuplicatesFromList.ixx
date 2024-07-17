// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_Coding_DeleteDuplicatesFromList;

import jpt.HashMap;
import jpt.Utilities;

struct ListNode 
{
	int val;
	ListNode *next;
	ListNode() : val(0), next(nullptr) {}
	ListNode(int x) : val(x), next(nullptr) {}
	ListNode(int x, ListNode *next) : val(x), next(next) {}
};

ListNode* DeleteDuplicates(ListNode* head)
{
	jpt::HashMap<int, int> counter;

	// Add all values and occurrences to the map
	ListNode* pCurrent = head;
	while (pCurrent)
	{
		++counter[pCurrent->val];
		pCurrent = pCurrent->next;
	}

	// Loop through the list again, and delete any nodes with a count greater than 1
	pCurrent = head;
	ListNode* pPrevious = nullptr;
	while (pCurrent)
	{
		// If the count is greater than 1, delete the node
		if (counter[pCurrent->val] > 1)
		{
			if (pPrevious)
			{
				pPrevious->next = pCurrent->next;
				delete pCurrent;
				pCurrent = pPrevious->next;
			}
			else
			{
				head = pCurrent->next;
				delete pCurrent;
				pCurrent = head;
			}
		}
		// Otherwise, move to the next node
		else
		{
			pPrevious = pCurrent;
			pCurrent = pCurrent->next;
		}
	}

	return head;
}

export bool UnitTests_Coding_DeleteDuplicatesFromList()
{
	ListNode* pHead = new ListNode(1);
	pHead->next = new ListNode(1);
	pHead->next->next = new ListNode(1);
	pHead->next->next->next = new ListNode(2);
	pHead->next->next->next->next = new ListNode(3);
	pHead->next->next->next->next->next = new ListNode(3);
	pHead->next->next->next->next->next->next = new ListNode(4);

	pHead = DeleteDuplicates(pHead);

	JPT_ENSURE(pHead->val == 2);
	JPT_ENSURE(pHead->next->val == 4);

	while (pHead)
	{
		ListNode* pNext = pHead->next;
		delete pHead;
		pHead = pNext;
	}

	return true;
}