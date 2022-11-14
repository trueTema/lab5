#pragma once
#include <exception>
#include <string>
enum except_type { MemoryAllocateError, IndexOutOfRange, SizeBelowZero, NegativeRange, 
	IncorrectComparator, IncorrectRange, IncorrectValue, IncorrectInputFormat, IncorrectIterator, 
	NoRequiredArgument, UnknownFlag, UnknownCommand, MutuallyExclusive, TooManySort, EmptySequence};
#include<iostream>

class SetException {
private:
	except_type id;
public:
	SetException(except_type id) { this->id = id; }
	const char* message() {
		if (id == MemoryAllocateError) return "Cannot allocate memory";
		if (id == IndexOutOfRange) return "Subscirpt index out of range";
		if (id == SizeBelowZero) return "Size below zero";
		if (id == NegativeRange) return "Range is less than zero";
		if (id == IncorrectComparator) return "Incorrect sorting comparator";
		if (id == IncorrectRange) return "The size of sequence isn't a power of 2";
		if (id == IncorrectValue) return "Not allowed parameter value";
		if (id == NoRequiredArgument) return "No required argument for command";
		if (id == UnknownFlag) return "Unknown flag name for this command";
		if (id == UnknownCommand) return "Unknown command name";
		if (id == IncorrectInputFormat) return "Incorrect Input format";
		if (id == MutuallyExclusive) return "There are two or more mutually exclusive arguments";
		if (id == TooManySort) return "You cannot compare too much function at the same time.";
		if (id == EmptySequence) return "Your sequence is empty";
	}
};