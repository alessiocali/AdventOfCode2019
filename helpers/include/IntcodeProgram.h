#pragma once

#include <vector>
#include <unordered_map>
#include <memory>
#include <string>
#include <iostream>

/****************************************************************************************************
		BRIEF TERMINOLOGY

- IntCode : A number composing an Intcode Program
- Program : A sequence of Intcodes
- InstructionCode : An Intcode which can be decomposed in an OpCode and zero or more ParameterModes
- OpCode : Two digits representing an Instruction to execute
- ParameterMode : A single digit representing how to interpret subsequent Intcode

*****************************************************************************************************/

class IntCodeProgram
{
public:
	enum class ExecutionStatus
	{
		Continue,
		Jump,
		Halt
	};

	struct InitData
	{
		std::uint32_t noun;
		std::uint32_t verb;
	};

	enum class OpCode : int
	{
		ADD = 1,
		MUL = 2,
		IN_ = 3,
		OU_ = 4,
		JT_ = 5,
		JF_ = 6,
		LT_ = 7,
		EQU = 8,
		HLT = 99
	};

	enum class ParameterMode : int
	{
		POS = 0,
		IMM = 1
	};

	using IntCode = int;
	using ProgramData = std::vector<IntCode>;

	explicit IntCodeProgram(std::string filename);

	void SetNounAndVerb(InitData init);
	void Execute();

	inline void SetOutputStream(std::shared_ptr<std::ostream> ostream) { m_OutputStream = ostream; }
	inline void SetInputStream(std::shared_ptr<std::istream> istream) { m_InputStream = istream; }

	inline bool IsValid() const { return !m_Program.empty(); }
	inline IntCode GetIntCodeAt(std::size_t idx) const { return m_Program[idx]; }

private:
	using ProgramIterator = ProgramData::iterator;
	using InstructionPtr = ExecutionStatus(IntCodeProgram::*)(ProgramIterator&);
	using InstructionSet = std::unordered_map<OpCode, InstructionPtr>;

	// Used for static init of the InstructionSet
	class InstructionSetHolder
	{
	public:
		InstructionSetHolder();
		inline const InstructionSet& GetInstructionSet() const { return m_InstructionSet; }

	private:
		InstructionSet m_InstructionSet;
	};

	ExecutionStatus Process(ProgramIterator& it);
	IntCode GetValueFromParameterMode(ParameterMode mode, IntCode intCode) const;

	static bool IsIntCodeAnInstructionCode(IntCode intCode);
	static bool IsValueAParameterMode(int value);

	static std::vector<ParameterMode> ExtractParameterModes(IntCode value, std::size_t count);

	ExecutionStatus Add(ProgramIterator& it);
	ExecutionStatus Mul(ProgramIterator& it);
	ExecutionStatus Input(ProgramIterator& it);
	ExecutionStatus Output(ProgramIterator& it);
	ExecutionStatus JumpIfTrue(ProgramIterator& it);
	ExecutionStatus JumpIfFalse(ProgramIterator& it);
	ExecutionStatus LessThan(ProgramIterator& it);
	ExecutionStatus Equals(ProgramIterator& it);
	ExecutionStatus Halt(ProgramIterator& it) { return ExecutionStatus::Halt; }

	template<typename IntCodeTest>
	ExecutionStatus InternalJump(ProgramIterator& it, IntCodeTest test);

	template<typename IntCodeComparison>
	ExecutionStatus InternalCompare(ProgramIterator& it, IntCodeComparison compare);

	static const InstructionSet& GetInstructionSet() 
	{
		static const InstructionSetHolder ms_InstructionSetHolder;
		return ms_InstructionSetHolder.GetInstructionSet(); 
	}

	ProgramData m_Program;

	inline std::ostream& GetOutputStream() { return m_OutputStream ? *m_OutputStream : std::cout; }
	inline std::istream& GetInputStream() { return m_InputStream ? *m_InputStream : std::cin; }
	
	std::shared_ptr<std::ostream> m_OutputStream;
	std::shared_ptr<std::istream> m_InputStream;
};