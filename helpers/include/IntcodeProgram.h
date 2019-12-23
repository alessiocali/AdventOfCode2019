#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>

class IntcodeProgram
{
public:
	struct Init
	{
		std::uint32_t noun;
		std::uint32_t verb;
	};

	using ProgramData = std::vector<std::uint32_t>;

	explicit IntcodeProgram(std::string filename, Init init)
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

	template<typename Functor>
	void PreProcess(Functor f)
	{
		f(m_Program);
	}

	void Execute()
	{
		ProgramIterator it = m_Program.begin();
		ProgramIterator end = m_Program.end();
		while (it != end)
		{
			if (!Process(it))
			{
				break;
			}
			it++;
		}
	}

	inline bool IsValid() const { return !m_Program.empty(); }
	inline std::uint32_t GetOpCodeAt(std::size_t idx) const { return m_Program[idx]; }

private:
	using ProgramIterator = std::vector<std::uint32_t>::iterator;

	bool Process(ProgramIterator& it)
	{
		switch (*it)
		{
		case OPCODE_ADD:
		{
			std::uint32_t in1 = *(++it);
			std::uint32_t in2 = *(++it);
			std::uint32_t out = *(++it);
			Add(in1, in2, out);
			return true;
		}
		case OPCODE_MUL:
		{
			std::uint32_t in1 = *(++it);
			std::uint32_t in2 = *(++it);
			std::uint32_t out = *(++it);
			Mul(in1, in2, out);
			return true;
		}
		case OPCODE_HLT:
			return false;
		default:
			std::cerr << "Unexpected OPCODE " << *it << " at position " << std::distance(m_Program.begin(), it) << std::endl;
			return false;
		}
	}

	void Add(std::uint32_t in1, std::uint32_t in2, std::uint32_t out)
	{
		m_Program[out] = m_Program[in1] + m_Program[in2];
	}

	void Mul(std::uint32_t in1, std::uint32_t in2, std::uint32_t out)
	{
		m_Program[out] = m_Program[in1] * m_Program[in2];
	}

	ProgramData m_Program;

	static constexpr std::uint32_t OPCODE_ADD = 1;
	static constexpr std::uint32_t OPCODE_MUL = 2;
	static constexpr std::uint32_t OPCODE_HLT = 99;
};