
#ifndef M68060INSTRUCTIONLENGTHDECODER_H
#define M68060INSTRUCTIONLENGTHDECODER_H

#include "../Types.h"

typedef struct
{
	// A 680x0 instruction is made up of:
	// 1 OpWord
	// 0-2 Special Operand Specifier words
	// 0-6 Immediate/Source EA Extension words
	// 0-6 Destination EA Extension words

	// The variables below count the number of words of each type

	uint numSpecialOperandSpecifierWords;
	uint numSourceEAExtensionWords;
	uint numDestinationEAExtensionWords;

	uint totalWords; // Total number of words in instruction, including the OpWord
	
	const char* description; // Approximate instruction description, mainly for debugging purposes. Example: "CMP Dn,<ea>"

} InstructionLength;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// This function attempts to decode length of the next instruction from a stream of instruction words.
//
// Usually the full length of the instruction can be determined just from the first word, but sometimes
//  subsequent words within the instruction also need to be inspected.
//
// If you provide this function with too few instruction words, so that it is unable to decode the total instruction length,
//  it will return false. When this happens you should add more instruction words to your buffer and try again.
// If the instruction words form an invalid instruction, the function will return true, but mnemonic
//  will be a null pointer, instructionLength.num*Words will all be zero, and and instructionLength.totalWords will be 1.
// Finally, if the instruction words are indeed the first words of a valid instruction, and you provide enough to decode the
//  total instruction length, the function will return true, mnemonic will be non-null, and the sizes in the
//  instructionLength buffer will be accurate.
//
// If the buffer contains a complete 680x0 instruction then this function is guaranteed to return its length.
// All 680x0 instructions less than 16 words in size, so if you always present 16+ words to this function it will always
//  succeed in decoding the instruction's length and respond with a zero word counts.
//
// Providing the decoder with a very large buffer (say, the entire remainder of the program) is perfectly valid and
//  will not affect performance or stability at all.

bool decodeInstructionLengthFromInstructionWords(const uint16_t* instructionWords, uint numInstructionWordsAvailable,
	InstructionLength* instructionLength);

#endif
