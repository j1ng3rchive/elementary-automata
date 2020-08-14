#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char getBit(char* array, unsigned long index) {
	unsigned long arrIndex = index / 8;
	char bitIndex = index & 7;
	char value = array[arrIndex];
	char bit = value >> bitIndex & 1;
	return bit;
}
char setBit(char* array, unsigned long index, char bit) {
	unsigned long arrIndex = index / 8;
	char bitIndex = index & 7;
	char mask = 1 << bitIndex;
	if(bit) {
		array[arrIndex] = array[arrIndex] | mask;
	} else {
		array[arrIndex] = array[arrIndex] & (255 ^ mask);
	}
}

unsigned long update(char rule, char length, unsigned long state) {
	unsigned long sum = 0;
	char index = length - 1;
	while(index >= 0) {
		sum *= 2;
		if(index == length - 1) {
			sum += rule >> (4*(state & 1) + (state >> (length - 2))) & 1;
		} else if(index == 0) {
			sum += rule >> (2*(state & 3) + (state >> (length - 1))) & 1;
		} else {
			sum += rule >> ((state >> (index - 1)) & 7) & 1;
		}
		index--;
	}
	return sum;
}

unsigned long findMaxCycleLength(char rule, char length) {
	unsigned long maxLength = 0;
	unsigned long state;
	unsigned long cycleLength;
	unsigned long backtrackState;
	char* allTime = calloc(1 << (length - 3), sizeof(char));
	char* currentCycle = calloc(1 << (length - 3), sizeof(char));
	for(unsigned long startState = 0; startState < (1 << length - 1); startState++) {
		state = startState;
		cycleLength = 0;
		//Follow the state cycle
		while(!getBit(allTime, state)) {
			setBit(allTime, state, 1);
			setBit(currentCycle, state, 1);
			cycleLength++;
			state = update(rule, length, state);
		}
		//We've found a state we've seen before!
		backtrackState = startState;
		if(getBit(currentCycle, state)) {
			//We found a new cycle; backtrack from the beginning to see when it was first found.
			while(backtrackState != state) {
				cycleLength--;
				setBit(currentCycle, backtrackState, 0);
				backtrackState = update(rule, length, backtrackState);
			}
			if(cycleLength > maxLength) {
				maxLength = cycleLength;
			}
		}
		//Clear all the set bits in currentCycle to prepare for a new cycle
		while(getBit(currentCycle, backtrackState)) {
			setBit(currentCycle, backtrackState, 0);
			backtrackState = update(rule, length, backtrackState);
		}
	}
	free(allTime);
	free(currentCycle);
	return maxLength;
}

int main(int argc, char* argv[]) {
	char* type = argc > 1 ? argv[1] : "single";
	char length = argc > 2 ? atoi(argv[2]) : 4;
	char rule = argc > 3 ? atoi(argv[3]) : 110;
	printf("Operation: %s. Rule: %d. Length: %d\n", type, rule, length);
	if(strcmp(type, "upto") == 0) {
		printf("Maximum Cycle Lengths (starting with length 3):\n");
		for(int i = 3; i <= length; i++) {
			printf("%d\n", findMaxCycleLength(rule, i));
		}
		printf("Done.\n");
	} else if(strcmp(type, "rule") == 0){
		char maxRule = 0;
		unsigned long maxLength = 0;
		for(char ruleIndex = 0;; ruleIndex++) {
			unsigned long cycleLength = findMaxCycleLength(ruleIndex, length);
			if(cycleLength > maxLength) {
				maxLength = cycleLength;
				maxRule = ruleIndex;
			}
			if(ruleIndex == -1) {
				break;
			}
		}
		printf("Maximum Cycle Length: %d for Rule: %d", maxLength, maxRule);
	} else if(strcmp(type, "ruleupto") == 0) {
		printf("Maximum Cycle Lengths, Rules (starting with length 3):\n");
		for(int i = 3; i <= length; i++) {
			char maxRule = 0;
			unsigned long maxLength = 0;
			for(char ruleIndex = 0;; ruleIndex++) {
				unsigned long cycleLength = findMaxCycleLength(ruleIndex, i);
				if(cycleLength > maxLength) {
					maxLength = cycleLength;
					maxRule = ruleIndex;
				}
				printf("Rule: %d                            \r", ruleIndex);
				if(ruleIndex == -1) {
					break;
				}
			}
			printf("%d, %d\n", maxLength, maxRule);
		}
		printf("Done.\n");
	} else {
		printf("Maximum Cycle Length: %d", findMaxCycleLength(rule, length));
	}
	printf("\n");
	scanf("Why is this necessary?\n");
	return 0;
}