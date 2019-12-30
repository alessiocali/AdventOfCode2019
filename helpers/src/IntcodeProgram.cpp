#include <IntcodeProgram.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_set>
#include <cassert>
#include <limits>

void IntCodeMemory::Reset(IntCodeProgram initialProgram)
{
	m_SequentialMemory.clear();
	m_SequentialMemory = std::move(initialProgram);
	m_UnboundedMemory.clear();
}

void IntCodeMemory::StoreValue(const IntCodeAddress& address, IntCodeValue value)
{
	assert(address >= 0);

	if (IsAddressInSequentialMemoryRange(address))
	{
		m_SequentialMemory[address.convert_to<std::size_t>()] = std::move(value);
	}
	else
	{
		m_UnboundedMemory[address] = value;
	}
}

IntCodeValue IntCodeMemory::ReadValue(const IntCodeAddress& address) const
{
	assert(address >= 0);

	if (IsAddressInSequentialMemoryRange(address))
	{
		return m_SequentialMemory[address.convert_to<std::size_t>()];
	}
	else
	{
		return m_UnboundedMemory.count(address) > 0 ? m_UnboundedMemory.at(address) : 0;
	}
}

bool IntCodeMemory::IsAddressInSequentialMemoryRange(const IntCodeAddress& address) const
{
	if (address > IntCodeAddress(std::numeric_limits<std::size_t>::max()))
	{
		return false;
	}

	return address.convert_to<std::size_t>() < m_SequentialMemory.size();
}

IntCodeComputer::InstructionSetHolder::InstructionSetHolder()
{
	m_InstructionSet[OpCode::ADD] = &IntCodeComputer::Add;
	m_InstructionSet[OpCode::MUL] = &IntCodeComputer::Mul;
	m_InstructionSet[OpCode::IN_] = &IntCodeComputer::Input;
	m_InstructionSet[OpCode::OU_] = &IntCodeComputer::Output;
	m_InstructionSet[OpCode::JT_] = &IntCodeComputer::JumpIfTrue;
	m_InstructionSet[OpCode::JF_] = &IntCodeComputer::JumpIfFalse;
	m_InstructionSet[OpCode::LT_] = &IntCodeComputer::LessThan;
	m_InstructionSet[OpCode::EQU] = &IntCodeComputer::Equals;
	m_InstructionSet[OpCode::RBS] = &IntCodeComputer::Rebase;
	m_InstructionSet[OpCode::HLT] = &IntCodeComputer::Halt;
}

bool IntCodeComputer::IsValueAnInstructionCode(IntCodeValue value)
{
	static const std::unordered_set<IntCodeValue> ValidOpCodes =
	{
		OpCode::ADD,
		OpCode::MUL,
		OpCode::IN_,
		OpCode::OU_,
		OpCode::JT_,
		OpCode::JF_,
		OpCode::LT_,
		OpCode::EQU,
		OpCode::RBS,
		OpCode::HLT
	};

	return ValidOpCodes.count(value % 100) > 0;
}

bool IntCodeComputer::IsValueAParameterMode(IntCodeValue value)
{
	static const std::unordered_set<IntCodeValue> ValidParameterModes =
	{
		ParameterMode::POS,
		ParameterMode::IMM,
		ParameterMode::REL
	};

	return ValidParameterModes.count(value) > 0;
}

std::vector<IntCodeComputer::ParameterMode> IntCodeComputer::ExtractParameterModes(IntCodeValue value, std::size_t count)
{
	std::vector<ParameterMode> parameterModes(count, ParameterMode::POS);
	
	value /= 100;
	std::size_t idx = 0;
	while (idx < count && value > 0)
	{
		IntCodeValue parameterModeValue = value % 10;
		const bool isValid = IsValueAParameterMode(parameterModeValue);
		
		if (!isValid)
		{
			std::cerr << "Unexpected Parameter Mode " << parameterModeValue << std::endl;
		}
		
		const ParameterMode parameterMode = isValid ? static_cast<ParameterMode>(parameterModeValue.convert_to<int>()) : ParameterMode::POS;
		parameterModes[idx++] = parameterMode;
		value /= 10;
	}

	return parameterModes;
}

IntCodeValue IntCodeComputer::GetValueFromParameterMode(ParameterMode mode, IntCodeValue value) const
{
	switch (mode)
	{
	case ParameterMode::IMM:
		return value;
	case ParameterMode::POS:
		return m_Memory.ReadValue(static_cast<IntCodeAddress>(value));
	case ParameterMode::REL:
		return m_Memory.ReadValue(static_cast<IntCodeAddress>(value) + m_RelativeBase);
	default:
		std::cerr << "Unrecognized ParameterMode " << static_cast<int>(mode) << std::endl;
		return 0;
	}
}

IntCodeAddress IntCodeComputer::GetAddressFromParameterMode(ParameterMode mode, IntCodeValue value) const
{
	switch (mode)
	{
	case ParameterMode::POS:
		return value;
	case ParameterMode::REL:
		return value + m_RelativeBase;
	case ParameterMode::IMM:
		std::cerr << "Cannot write on an Immediate" << std::endl;
	default:
		std::cerr << "Unrecognized ParameterMode " << static_cast<int>(mode) << std::endl;
		return 0;
	}
}

IntCodeProgram IntCodeComputer::LoadFromFile(const std::string& filename)
{
	IntCodeProgram program;

	std::ifstream input(filename);
	if (!input.is_open())
	{
		return program;
	}

	std::string programLine;
	while (std::getline(input, programLine))
	{
		std::stringstream programStringStream(programLine);
		std::string intCodeValueString;
		while (std::getline(programStringStream, intCodeValueString, ','))
		{
			program.emplace_back(intCodeValueString);
		}
	}

	return program;
}

IntCodeComputer::IntCodeComputer(std::string fileName)
	: m_OriginalProgram(LoadFromFile(fileName))
{
	Reset();
}

void IntCodeComputer::SetNounAndVerb(InitData initData)
{
	m_Memory.StoreValue(1, initData.noun);
	m_Memory.StoreValue(2, initData.verb);
}

void IntCodeComputer::Reset()
{
	m_Memory.Reset(m_OriginalProgram);
	m_InstructionPointer = 0;
	m_RelativeBase = 0;
	m_Status = ExecutionStatus::NotStarted;
	
	m_InputStream.str(std::string());
	m_InputStream.clear();

	m_OutputStream.str(std::string());
	m_OutputStream.clear();
}

void IntCodeComputer::Execute()
{
	m_Status = ExecutionStatus::Running;

	while (IsRunning())
	{
		const ExecutionProgress status = ProcessCurrentInstruction();
		switch(status)
		{
		case ExecutionProgress::Halt:
			m_Status = ExecutionStatus::Halted;
			break;
		case ExecutionProgress::Pause:
			m_InstructionPointer = m_InstructionPointer + 1;
			m_Status = ExecutionStatus::Paused;
			break;
		case ExecutionProgress::Jump:
			continue;
		case ExecutionProgress::Continue:
			m_InstructionPointer = m_InstructionPointer + 1;
			break;
		default:
			std::cerr << "Unrecognized ExecutionStatus: " << static_cast<int>(status) << std::endl;
			m_Status = ExecutionStatus::Halted;
			break;
		}
	}
}

IntCodeComputer::ExecutionProgress IntCodeComputer::ProcessCurrentInstruction()
{
	const IntCodeValue& instruction = GetCurrentInstruction();
	if (!IsValueAnInstructionCode(instruction))
	{
		std::cerr << "Unexpected VALUE is not an INSTRUCTION_CODE: " << instruction << " at position " << m_InstructionPointer << std::endl;
		return ExecutionProgress::Halt;
	}

	OpCode opCode = GetCurrentOpCode();
	const InstructionSet& instructionSet = GetInstructionSet();
	if (instructionSet.count(opCode) && instructionSet.at(opCode))
	{
		const InstructionFnc instructionFnc = instructionSet.at(opCode);
		return (this->*instructionFnc)();
	}
	else
	{
		std::cerr << "Unexpected OPCODE " << instruction << " at position " << m_InstructionPointer << std::endl;
		return ExecutionProgress::Halt;
	}
}

IntCodeComputer::ExecutionProgress IntCodeComputer::Add()
{
	assert(GetCurrentOpCode() == OpCode::ADD);
	
	const std::vector<ParameterMode> parameterModes = ExtractParameterModes(GetCurrentInstruction(), 3);

	const IntCodeValue in1 = GetValueFromParameterMode(parameterModes[0], GetNextValueAndStepPointer());
	const IntCodeValue in2 = GetValueFromParameterMode(parameterModes[1], GetNextValueAndStepPointer());
	const IntCodeAddress out = GetAddressFromParameterMode(parameterModes[2], GetNextValueAndStepPointer());

	m_Memory.StoreValue(out, in1 + in2);

	return ExecutionProgress::Continue;
}

IntCodeComputer::ExecutionProgress IntCodeComputer::Mul()
{
	assert(GetCurrentOpCode() == OpCode::MUL);

	const std::vector<ParameterMode> parameterModes = ExtractParameterModes(GetCurrentInstruction(), 3);

	const IntCodeValue in1 = GetValueFromParameterMode(parameterModes[0], GetNextValueAndStepPointer());
	const IntCodeValue in2 = GetValueFromParameterMode(parameterModes[1], GetNextValueAndStepPointer());
	const IntCodeAddress out = GetAddressFromParameterMode(parameterModes[2], GetNextValueAndStepPointer());

	m_Memory.StoreValue(out, in1 * in2);

	return ExecutionProgress::Continue;
}

IntCodeComputer::ExecutionProgress IntCodeComputer::Input()
{
	assert(GetCurrentOpCode() == OpCode::IN_);

	const std::vector<ParameterMode> parameterModes = ExtractParameterModes(GetCurrentInstruction(), 1);
	
	IntCodeValue value;
	GetInputStream() >> value;

	IntCodeAddress out = GetAddressFromParameterMode(parameterModes[0], GetNextValueAndStepPointer());

	m_Memory.StoreValue(out, value);

	return ExecutionProgress::Continue;
}

IntCodeComputer::ExecutionProgress IntCodeComputer::Output()
{
	assert(GetCurrentOpCode() == OpCode::OU_);

	const std::vector<ParameterMode> parameterModes = ExtractParameterModes(GetCurrentInstruction(), 1);

	const IntCodeValue in1 = GetValueFromParameterMode(parameterModes[0], GetNextValueAndStepPointer());

	GetOutputStream() << in1 << std::endl;
	
	return m_PauseOnOutput ? ExecutionProgress::Pause : ExecutionProgress::Continue;
}

IntCodeComputer::ExecutionProgress IntCodeComputer::JumpIfTrue()
{
	return InternalJump([](IntCodeValue v) { return v != 0; });
}

IntCodeComputer::ExecutionProgress IntCodeComputer::JumpIfFalse()
{
	return InternalJump([](IntCodeValue v) { return v == 0; });
}

template<typename IntCodeTest>
IntCodeComputer::ExecutionProgress IntCodeComputer::InternalJump(IntCodeTest test)
{
	OpCode opCode = GetCurrentOpCode();
	assert( opCode == OpCode::JF_ || opCode == OpCode::JT_ );

	const std::vector<ParameterMode> parameterModes = ExtractParameterModes(GetCurrentInstruction(), 2);

	const IntCodeValue in1 = GetValueFromParameterMode(parameterModes[0], GetNextValueAndStepPointer());
	const IntCodeValue in2 = GetValueFromParameterMode(parameterModes[1], GetNextValueAndStepPointer());

	if (test(in1))
	{
		m_InstructionPointer = in2;
		return ExecutionProgress::Jump;
	}
	else
	{
		return ExecutionProgress::Continue;
	}
}

IntCodeComputer::ExecutionProgress IntCodeComputer::LessThan()
{
	return InternalCompare([](IntCodeValue v1, IntCodeValue v2) { return v1 < v2;  });
}

IntCodeComputer::ExecutionProgress IntCodeComputer::Equals()
{
	return InternalCompare([](IntCodeValue v1, IntCodeValue v2) { return v1 == v2; });
}

template<typename IntCodeComparison>
IntCodeComputer::ExecutionProgress IntCodeComputer::InternalCompare(IntCodeComparison comparison)
{
	OpCode opCode = GetCurrentOpCode();
	assert( opCode == OpCode::LT_ || opCode == OpCode::EQU );

	std::vector<ParameterMode> parameterModes = ExtractParameterModes(GetCurrentInstruction(), 3);

	const IntCodeValue in1 = GetValueFromParameterMode(parameterModes[0], GetNextValueAndStepPointer());
	const IntCodeValue in2 = GetValueFromParameterMode(parameterModes[1], GetNextValueAndStepPointer());
	const IntCodeAddress out = GetAddressFromParameterMode(parameterModes[2], GetNextValueAndStepPointer());

	m_Memory.StoreValue(out, comparison(in1, in2) ? 1 : 0);
	
	return ExecutionProgress::Continue;
}

IntCodeComputer::ExecutionProgress IntCodeComputer::Rebase()
{
	assert(GetCurrentOpCode() == OpCode::RBS);

	std::vector<ParameterMode> parameterModes = ExtractParameterModes(GetCurrentInstruction(), 1);
	const IntCodeValue in = GetValueFromParameterMode(parameterModes[0], GetNextValueAndStepPointer());

	m_RelativeBase = m_RelativeBase + static_cast<IntCodeAddress>(in);

	return ExecutionProgress::Continue;
}