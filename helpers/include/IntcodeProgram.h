#pragma once

#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include <boost/multiprecision/cpp_int.hpp>

using IntCodeAddress = boost::multiprecision::cpp_int;
using IntCodeValue = boost::multiprecision::cpp_int;
using IntCodeProgram = std::vector<IntCodeValue>;

class IntCodeMemory
{
public:
	inline bool IsLoaded() const { return !(m_SequentialMemory.empty() && m_UnboundedMemory.empty()); }

	// Implementation choice: ReadValue returns by value (no pun intended)
	// This is because reading from an arbitrary unbounded address has to return 0
	// if such address has never been written to. 
	//
	// The alternative would have been to have a non-const read which puts the 0 on
	// read, which although maybe more efficent would have been hacky at best.

	void Reset(IntCodeProgram initialProgram);
	void StoreValue(const IntCodeAddress& address, IntCodeValue value);
	IntCodeValue ReadValue(const IntCodeAddress& address) const;

private:
	bool IsAddressInSequentialMemoryRange(const IntCodeAddress& address) const;

	std::vector<IntCodeValue> m_SequentialMemory;
	std::unordered_map<IntCodeAddress, IntCodeValue> m_UnboundedMemory;
};

class IntCodeComputer
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
		IntCodeValue noun;
		IntCodeValue verb;
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
		RBS = 9,
		HLT = 99
	};

	enum class ParameterMode : int
	{
		POS = 0,
		IMM = 1,
		REL = 2
	};

	explicit IntCodeComputer(std::string filename);

	IntCodeComputer(const IntCodeComputer& other) = delete;
	IntCodeComputer& operator=(const IntCodeComputer& other) = delete;

	// The internal streams are trivially movable, but not copyable
	IntCodeComputer(IntCodeComputer&& other) = default;
	IntCodeComputer& operator=(IntCodeComputer&& other) = default;

	void SetNounAndVerb(InitData init);
	void Reset();
	void Execute();

	template<typename T>
	inline bool FeedInput(T input) { return bool(m_InputStream << input << std::endl); }

	template<typename T>
	inline bool GetOutput(T& output) { return bool(m_OutputStream >> output); }

	inline void SetPauseOnOutput(bool pauseOnOutput) { m_PauseOnOutput = pauseOnOutput; }
	inline bool IsValid() const { return m_Memory.IsLoaded(); }
	inline bool IsRunning() const { return m_Status == ExecutionStatus::Running; }
	inline bool IsHalted() const { return m_Status == ExecutionStatus::Halted; }
	inline IntCodeValue GetValueAt(IntCodeAddress address) const { return m_Memory.ReadValue(address); }

private:
	using InstructionFnc = ExecutionProgress(IntCodeComputer::*)();
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

	static IntCodeProgram LoadFromFile(const std::string& filename);

	ExecutionProgress ProcessCurrentInstruction();
	IntCodeValue GetValueFromParameterMode(ParameterMode mode, IntCodeValue value) const;
	IntCodeAddress GetAddressFromParameterMode(ParameterMode mode, IntCodeValue value) const;

	static bool IsValueAnInstructionCode(IntCodeValue value);
	static bool IsValueAParameterMode(IntCodeValue value);

	static std::vector<ParameterMode> ExtractParameterModes(IntCodeValue value, std::size_t count);

	ExecutionProgress Add();
	ExecutionProgress Mul();
	ExecutionProgress Input();
	ExecutionProgress Output();
	ExecutionProgress JumpIfTrue();
	ExecutionProgress JumpIfFalse();
	ExecutionProgress LessThan();
	ExecutionProgress Equals();
	ExecutionProgress Rebase();
	ExecutionProgress Halt() { return ExecutionProgress::Halt; }

	template<typename IntCodeTest>
	ExecutionProgress InternalJump(IntCodeTest test);

	template<typename IntCodeComparison>
	ExecutionProgress InternalCompare(IntCodeComparison compare);

	static const InstructionSet& GetInstructionSet() 
	{
		static const InstructionSetHolder ms_InstructionSetHolder;
		return ms_InstructionSetHolder.GetInstructionSet(); 
	}

	inline std::ostream& GetOutputStream() { return m_OutputStream; }
	inline std::istream& GetInputStream() { return m_InputStream; }

	inline const IntCodeValue GetCurrentInstruction() const { return m_Memory.ReadValue(m_InstructionPointer); }
	inline const IntCodeValue GetNextValueAndStepPointer() { return m_Memory.ReadValue(++m_InstructionPointer); }
	inline OpCode GetCurrentOpCode() const { return static_cast<OpCode>((GetCurrentInstruction() % 100).convert_to<int>()); }

	const IntCodeProgram m_OriginalProgram;
	
	IntCodeMemory		m_Memory;
	IntCodeAddress		m_InstructionPointer = 0;
	IntCodeAddress		m_RelativeBase = 0;
	ExecutionStatus		m_Status = ExecutionStatus::NotStarted;
	bool				m_PauseOnOutput = false;

	std::stringstream	m_OutputStream;
	std::stringstream	m_InputStream;
};