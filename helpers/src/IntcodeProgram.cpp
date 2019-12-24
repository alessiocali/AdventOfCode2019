#include <IntcodeProgram.h>

#include <iostream>
#include <fstream>
#include <sstream>

const IntcodeProgram::InstructionSetHolder IntcodeProgram::ms_InstructionSetHolder = IntcodeProgram::InstructionSetHolder();

IntcodeProgram::InstructionSetHolder::InstructionSetHolder()
{
	static constexpr OpCode OPCODE_ADD = 1;
	static constexpr OpCode OPCODE_MUL = 2;
	static constexpr OpCode OPCODE_HLT = 99;

	m_InstructionSet[OPCODE_ADD] = std::make_unique<Add>();
	m_InstructionSet[OPCODE_MUL] = std::make_unique<Mul>();
	m_InstructionSet[OPCODE_HLT] = std::make_unique<Halt>();
}

IntcodeProgram::IntcodeProgram(std::string filename, Init init)
{
	std::ifstream input(filename);
	if (!input.is_open())
	{
		return;
	}

	std::string opCodeString;
	while (std::getline(input, opCodeString, ','))
	{
		std::uint32_t opCode = static_cast<std::uint32_t>(std::stoi(opCodeString));
		m_Program.push_back(opCode);
	}

	m_Program[1] = init.noun;
	m_Program[2] = init.verb;
}

void IntcodeProgram::Execute()
{
	ProgramIterator it = m_Program.begin();
	ProgramIterator end = m_Program.end();
	while (it != end)
	{
		if (Process(it) == ExecutionStatus::Halt)
		{
			break;
		}
		it++;
	}
}

IntcodeProgram::ExecutionStatus IntcodeProgram::Process(ProgramIterator& it)
{
	OpCode opCode = *it;
	const InstructionSet& instructionSet = GetInstructionSet();
	if (instructionSet.count(opCode) && instructionSet.at(opCode))
	{
		const Instruction& instruction = *instructionSet.at(opCode);
		return instruction.Execute(it, m_Program);
	}
	else
	{
		std::cerr << "Unexpected OPCODE " << *it << " at position " << std::distance(m_Program.begin(), it) << std::endl;
		return ExecutionStatus::Halt;
	}
}

IntcodeProgram::ExecutionStatus IntcodeProgram::Add::Execute(ProgramIterator& it, ProgramData& program) const
{
	OpCode in1 = *(++it);
	OpCode in2 = *(++it);
	OpCode out = *(++it);
	program[out] = program[in1] + program[in2];
	return ExecutionStatus::Continue;
}

IntcodeProgram::ExecutionStatus IntcodeProgram::Mul::Execute(ProgramIterator& it, ProgramData& program) const
{
	OpCode in1 = *(++it);
	OpCode in2 = *(++it);
	OpCode out = *(++it);
	program[out] = program[in1] * program[in2];
	return ExecutionStatus::Continue;
}