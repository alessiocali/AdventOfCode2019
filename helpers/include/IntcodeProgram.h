#pragma once

#include <vector>
#include <unordered_map>
#include <memory>
#include <string>
#include <iostream>
#include <sstream>

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
	enum class ExecutionProgress
	{
		Continue,
		Jump,
		Pause,
		Halt
	};

	enum class ExecutionStatus
	{
		NotStarted,
		Running,
		Paused,
		Halted
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

	IntCodeProgram(const IntCodeProgram& other) = delete;
	IntCodeProgram& operator=(const IntCodeProgram& other) = delete;

	// The internal streams are trivially movable, but not copyable
	IntCodeProgram(IntCodeProgram&& other) = default;
	IntCodeProgram& operator=(IntCodeProgram&& other) = default;

	void SetNounAndVerb(InitData init);
	void Reset();
	void Execute();

	template<typename T>
	inline bool FeedInput(T input) { return bool(m_InputStream << input << std::endl); }

	template<typename T>
	inline bool GetOutput(T& output) { return bool(m_OutputStream >> output); }

	inline void SetPauseOnOutput(bool pauseOnOutput) { m_PauseOnOutput = pauseOnOutput; }
	inline bool IsValid() const { return !m_Program.empty(); }
	inline bool IsRunning() const { return m_Status == ExecutionStatus::Running; }
	inline bool IsHalted() const { return m_Status == ExecutionStatus::Halted; }
	inline IntCode GetIntCodeAt(std::size_t idx) const { return m_Program[idx]; }

private:
	using ProgramIterator = ProgramData::iterator;
	using InstructionFnc = ExecutionProgress(IntCodeProgram::*)(ProgramIterator&);
	using InstructionSet = std::unordered_map<OpCode, InstructionFnc>;

	// Used for static init of the InstructionSet
	class InstructionSetHolder
	{
	public:
		InstructionSetHolder();
		inline const InstructionSet& GetInstructionSet() const { return m_InstructionSet; }

	private:
		InstructionSet m_InstructionSet;
	};

	ExecutionProgress Process(ProgramIterator& it);
	IntCode GetValueFromParameterMode(ParameterMode mode, IntCode intCode) const;

	static ProgramData LoadFromFile(const std::string& filename);

	static bool IsIntCodeAnInstructionCode(IntCode intCode);
	static bool IsValueAParameterMode(int value);

	static std::vector<ParameterMode> ExtractParameterModes(IntCode value, std::size_t count);

	ExecutionProgress Add(ProgramIterator& it);
	ExecutionProgress Mul(ProgramIterator& it);
	ExecutionProgress Input(ProgramIterator& it);
	ExecutionProgress Output(ProgramIterator& it);
	ExecutionProgress JumpIfTrue(ProgramIterator& it);
	ExecutionProgress JumpIfFalse(ProgramIterator& it);
	ExecutionProgress LessThan(ProgramIterator& it);
	ExecutionProgress Equals(ProgramIterator& it);
	ExecutionProgress Halt(ProgramIterator& it) { return ExecutionProgress::Halt; }

	template<typename IntCodeTest>
	ExecutionProgress InternalJump(ProgramIterator& it, IntCodeTest test);

	template<typename IntCodeComparison>
	ExecutionProgress InternalCompare(ProgramIterator& it, IntCodeComparison compare);

	static const InstructionSet& GetInstructionSet() 
	{
		static const InstructionSetHolder ms_InstructionSetHolder;
		return ms_InstructionSetHolder.GetInstructionSet(); 
	}

	inline std::ostream& GetOutputStream() { return m_OutputStream; }
	inline std::istream& GetInputStream() { return m_InputStream; }

	const ProgramData	m_OriginalProgram;
	ProgramData			m_Program;
	
	// I couldn't store a ProgramIterator because it gets invalidated on move/copy
	// and I'm too lazy to write a move ctor just for that.
	std::size_t			m_InstructionPointer;	
	ExecutionStatus		m_Status = ExecutionStatus::NotStarted;
	bool				m_PauseOnOutput = false;

	std::stringstream	m_OutputStream;
	std::stringstream	m_InputStream;
};