#include <IntcodeProgram.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_set>
#include <cassert>

IntCodeProgram::InstructionSetHolder::InstructionSetHolder()
{
	m_InstructionSet[OpCode::ADD] = &IntCodeProgram::Add;
	m_InstructionSet[OpCode::MUL] = &IntCodeProgram::Mul;
	m_InstructionSet[OpCode::IN_] = &IntCodeProgram::Input;
	m_InstructionSet[OpCode::OU_] = &IntCodeProgram::Output;
	m_InstructionSet[OpCode::JT_] = &IntCodeProgram::JumpIfTrue;
	m_InstructionSet[OpCode::JF_] = &IntCodeProgram::JumpIfFalse;
	m_InstructionSet[OpCode::LT_] = &IntCodeProgram::LessThan;
	m_InstructionSet[OpCode::EQU] = &IntCodeProgram::Equals;
	m_InstructionSet[OpCode::HLT] = &IntCodeProgram::Halt;
}

bool IntCodeProgram::IsIntCodeAnInstructionCode(IntCode intCode)
{
	static const std::unordered_set<IntCode> ValidOpCodes =
	{
		static_cast<IntCode>(OpCode::ADD),
		static_cast<IntCode>(OpCode::MUL),
		static_cast<IntCode>(OpCode::IN_),
		static_cast<IntCode>(OpCode::OU_),
		static_cast<IntCode>(OpCode::JT_),
		static_cast<IntCode>(OpCode::JF_),
		static_cast<IntCode>(OpCode::LT_),
		static_cast<IntCode>(OpCode::EQU),
		static_cast<IntCode>(OpCode::HLT)
	};

	return ValidOpCodes.count(intCode % 100) > 0;
}

bool IntCodeProgram::IsValueAParameterMode(int value)
{
	static const std::unordered_set<int> ValidParameterModes =
	{
		static_cast<int>(ParameterMode::POS),
		static_cast<int>(ParameterMode::IMM)
	};

	return ValidParameterModes.count(value) > 0;
}

std::vector<IntCodeProgram::ParameterMode> IntCodeProgram::ExtractParameterModes(IntCode value, std::size_t count)
{
	std::vector<ParameterMode> parameterModes(count, ParameterMode::POS);
	
	// Clear OpCode
	value /= 100;
	std::size_t idx = 0;
	while (idx < count && value > 0)
	{
		int parameterModeValue = value % 10;
		const bool isValid = IsValueAParameterMode(parameterModeValue);
		
		if (!isValid)
		{
			std::cerr << "Unexpected Parameter Mode " << parameterModeValue << std::endl;
		}
		
		const ParameterMode parameterMode = isValid ? static_cast<ParameterMode>(parameterModeValue) : ParameterMode::POS;
		parameterModes[idx++] = parameterMode;
		value /= 10;
	}

	return parameterModes;
}

IntCodeProgram::IntCode IntCodeProgram::GetValueFromParameterMode(ParameterMode mode, IntCode value) const
{
	switch (mode)
	{
	case ParameterMode::IMM:
		return value;
	case ParameterMode::POS:
		return m_Program[value];
	default:
		std::cerr << "Unrecognized ParameterMode " << static_cast<int>(mode) << std::endl;
		return 0;
	}
}

IntCodeProgram::ProgramData IntCodeProgram::LoadFromFile(const std::string& filename)
{
	ProgramData program;

	std::ifstream input(filename);
	if (!input.is_open())
	{
		return program;
	}

	std::string programString;
	while (std::getline(input, programString, ','))
	{
		IntCode intCode = static_cast<IntCode>(std::stoi(programString));
		program.push_back(intCode);
	}

	return program;
}

IntCodeProgram::IntCodeProgram(std::string fileName)
	: m_OriginalProgram(LoadFromFile(fileName))
{
	Reset();
}

void IntCodeProgram::SetNounAndVerb(InitData initData)
{
	m_Program[1] = initData.noun;
	m_Program[2] = initData.verb;
}

void IntCodeProgram::Reset()
{
	m_Program = m_OriginalProgram;
	m_InstructionPointer = 0;
	m_Status = ExecutionStatus::NotStarted;
	
	m_InputStream.str(std::string());
	m_InputStream.clear();

	m_OutputStream.str(std::string());
	m_OutputStream.clear();
}

void IntCodeProgram::Execute()
{
	m_Status = ExecutionStatus::Running;

	ProgramIterator it = m_Program.begin() + m_InstructionPointer;
	ProgramIterator end = m_Program.end();

	while (it != end && IsRunning())
	{
		const ExecutionProgress status = Process(it);
		switch(status)
		{
		case ExecutionProgress::Halt:
			m_Status = ExecutionStatus::Halted;
			break;
		case ExecutionProgress::Pause:
			it++;
			m_Status = ExecutionStatus::Paused;
			break;
		case ExecutionProgress::Jump:
			continue;
		case ExecutionProgress::Continue:
			it++;
			break;
		default:
			std::cerr << "Unrecognized ExecutionStatus: " << static_cast<int>(status) << std::endl;
			m_Status = ExecutionStatus::Halted;
			break;
		}
	}

	m_InstructionPointer = std::distance(m_Program.begin(), it);
}

IntCodeProgram::ExecutionProgress IntCodeProgram::Process(ProgramIterator& it)
{
	if (!IsIntCodeAnInstructionCode(*it))
	{
		std::cerr << "Unexpected INTCODE is not an INSTRUCTION_CODE: " << *it << " at position " << std::distance(m_Program.begin(), it) << std::endl;
		return ExecutionProgress::Halt;
	}

	OpCode opCode = static_cast<OpCode>(*it % 100);
	const InstructionSet& instructionSet = GetInstructionSet();
	if (instructionSet.count(opCode) && instructionSet.at(opCode))
	{
		const InstructionFnc instruction = instructionSet.at(opCode);
		return (this->*instruction)(it);
	}
	else
	{
		std::cerr << "Unexpected OPCODE " << *it << " at position " << std::distance(m_Program.begin(), it) << std::endl;
		return ExecutionProgress::Halt;
	}
}

IntCodeProgram::ExecutionProgress IntCodeProgram::Add(ProgramIterator& it)
{
	OpCode opCode = static_cast<OpCode>(*it % 100);
	assert(opCode == OpCode::ADD);
	
	const std::vector<ParameterMode> parameterModes = ExtractParameterModes(*it, 3);
	assert(parameterModes[2] == ParameterMode::POS);

	const IntCode in1 = GetValueFromParameterMode(parameterModes[0], *(++it));
	const IntCode in2 = GetValueFromParameterMode(parameterModes[1], *(++it));
	const IntCode out = *(++it);

	m_Program[out] = in1 + in2;

	return ExecutionProgress::Continue;
}

IntCodeProgram::ExecutionProgress IntCodeProgram::Mul(ProgramIterator& it)
{
	OpCode opCode = static_cast<OpCode>(*it % 100);
	assert(opCode == OpCode::MUL);

	const std::vector<ParameterMode> parameterModes = ExtractParameterModes(*it, 3);
	assert(parameterModes[2] == ParameterMode::POS);

	const IntCode in1Val = GetValueFromParameterMode(parameterModes[0], *(++it));
	const IntCode in2Val = GetValueFromParameterMode(parameterModes[1], *(++it));
	const IntCode out = *(++it);

	m_Program[out] = in1Val * in2Val;

	return ExecutionProgress::Continue;
}

IntCodeProgram::ExecutionProgress IntCodeProgram::Input(ProgramIterator& it)
{
	OpCode opCode = static_cast<OpCode>(*it % 100);
	assert(opCode == OpCode::IN_);

	const std::vector<ParameterMode> parameterModes = ExtractParameterModes(*it, 1);
	assert(parameterModes[0] == ParameterMode::POS);
	
	int value;
	GetInputStream() >> value;

	IntCode out = *(++it);
	m_Program[out] = static_cast<IntCode>(value);

	return ExecutionProgress::Continue;
}

IntCodeProgram::ExecutionProgress IntCodeProgram::Output(ProgramIterator& it)
{
	OpCode opCode = static_cast<OpCode>(*it % 100);
	assert(opCode == OpCode::OU_);

	const std::vector<ParameterMode> parameterModes = ExtractParameterModes(*it, 1);

	const IntCode in1 = GetValueFromParameterMode(parameterModes[0], *(++it));

	GetOutputStream() << static_cast<int>(in1) << std::endl;
	
	return m_PauseOnOutput ? ExecutionProgress::Pause : ExecutionProgress::Continue;
}

IntCodeProgram::ExecutionProgress IntCodeProgram::JumpIfTrue(ProgramIterator& it)
{
	return InternalJump(it, [](IntCode f) { return f != 0; });
}

IntCodeProgram::ExecutionProgress IntCodeProgram::JumpIfFalse(ProgramIterator& it)
{
	return InternalJump(it, [](IntCode f) { return f == 0; });
}

template<typename IntCodeTest>
IntCodeProgram::ExecutionProgress IntCodeProgram::InternalJump(ProgramIterator& it, IntCodeTest test)
{
	OpCode opCode = static_cast<OpCode>(*it % 100);
	assert( opCode == OpCode::JF_ || opCode == OpCode::JT_ );

	const std::vector<ParameterMode> parameterModes = ExtractParameterModes(*it, 2);

	const IntCode in1 = GetValueFromParameterMode(parameterModes[0], *(++it));
	const IntCode in2 = GetValueFromParameterMode(parameterModes[1], *(++it));

	if (test(in1))
	{
		it = m_Program.begin() + in2;
		return ExecutionProgress::Jump;
	}
	else
	{
		return ExecutionProgress::Continue;
	}
}

IntCodeProgram::ExecutionProgress IntCodeProgram::LessThan(ProgramIterator& it)
{
	return InternalCompare(it, [](IntCode f1, IntCode f2) { return f1 < f2;  });
}

IntCodeProgram::ExecutionProgress IntCodeProgram::Equals(ProgramIterator& it)
{
	return InternalCompare(it, [](IntCode f1, IntCode f2) { return f1 == f2; });
}

template<typename IntCodeComparison>
IntCodeProgram::ExecutionProgress IntCodeProgram::InternalCompare(ProgramIterator& it, IntCodeComparison comparison)
{
	OpCode opCode = static_cast<OpCode>(*it % 100);
	assert( opCode == OpCode::LT_ || opCode == OpCode::EQU );

	std::vector<ParameterMode> parameterModes = ExtractParameterModes(*it, 3);
	assert(parameterModes[2] == ParameterMode::POS);

	const IntCode in1 = GetValueFromParameterMode(parameterModes[0], *(++it));
	const IntCode in2 = GetValueFromParameterMode(parameterModes[1], *(++it));
	const IntCode out = *(++it);

	m_Program[out] = comparison(in1, in2) ? static_cast<IntCode>(1) : static_cast<IntCode>(0);
	
	return ExecutionProgress::Continue;
}