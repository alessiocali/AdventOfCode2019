#pragma once

#include <vector>
#include <unordered_map>
#include <memory>
#include <string>

class IntcodeProgram
{
public:
	enum class ExecutionStatus
	{
		Continue,
		Halt
	};

	struct Init
	{
		std::uint32_t noun;
		std::uint32_t verb;
	};

	using OpCode = std::uint32_t;
	using ProgramData = std::vector<OpCode>;

	explicit IntcodeProgram(std::string filename, Init init);

	template<typename Functor>
	void PreProcess(Functor f)
	{
		f(m_Program);
	}

	void Execute();

	inline bool IsValid() const { return !m_Program.empty(); }
	inline OpCode GetOpCodeAt(std::size_t idx) const { return m_Program[idx]; }

private:
	using ProgramIterator = std::vector<std::uint32_t>::iterator;

	ExecutionStatus Process(ProgramIterator& it);

	class Instruction 
	{
	public:
		virtual ExecutionStatus Execute(ProgramIterator& it, ProgramData& program) const = 0;
	};

	class Add : public Instruction
	{
		virtual ExecutionStatus Execute(ProgramIterator& it, ProgramData& program) const override;
	};

	class Mul : public Instruction
	{
		virtual ExecutionStatus Execute(ProgramIterator& it, ProgramData& program) const override;
	};

	class Halt : public Instruction
	{
		virtual ExecutionStatus Execute(ProgramIterator& it, ProgramData& program) const override { return ExecutionStatus::Halt; }
	};

	using InstructionPtr = std::unique_ptr<const Instruction>;
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

	static const InstructionSetHolder ms_InstructionSetHolder;
	static const InstructionSet& GetInstructionSet() { return ms_InstructionSetHolder.GetInstructionSet(); }

	ProgramData m_Program;
};