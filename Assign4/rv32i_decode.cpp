//***************************************************************************
//
//  Alex Marine (z1863905)
//  Assignment 4
//  CSCI 463-1
//  3/2/2021
//
//  I certify that this is my own work and where appropriate an extension
//  of the starter code provided for the assignment
//
//***************************************************************************

#include "memory.h"
#include "hex.h"
#include "rv32i_decode.h"
#include <cassert>

/**
 * @brief Decodes an insatruction by calling the appropriate render function
 *
 * @param addr Address of the instruction to decode
 * @param insn The instruction that will be decoded
 *
 * @return The decoded instruction as created from the corresponsing render function
 **********************************************************************************************/
std::string rv32i_decode::decode(uint32_t addr, uint32_t insn)
{
  //variable definitions to get funct3 and funct7 for instructions
  uint32_t funct3 = get_funct3(insn);
  uint32_t funct7 = get_funct7(insn);

  //if the instruction matches that for ecall, render the ecall
  if (insn == insn_ecall)
  {
    return render_ecall(insn);
  }
  //if the instruction matches that for ebreak, render the ebreak
  else if(insn == insn_ebreak)
  {
    return render_ebreak(insn);
  }

  //switch statement for opcode of the instruction
  switch (get_opcode(insn))
  {
    default:			return render_illegal_insn(insn);
    case opcode_lui:		return render_lui(insn);
    case opcode_auipc:		return render_auipc(insn);
    case opcode_jal:		return render_jal(addr, insn);
    case opcode_jalr:		return render_jalr(insn);
    //if opcode matches that of a btype, look at funct3 to differentiate
    case opcode_btype:
      switch (funct3)
      {
        default:		return render_illegal_insn(insn);
	case funct3_beq:	return render_btype(addr, insn, "beq");
	case funct3_bne:	return render_btype(addr, insn, "bne");
	case funct3_blt:	return render_btype(addr, insn, "blt");
	case funct3_bge:	return render_btype(addr, insn, "bge");
	case funct3_bltu:	return render_btype(addr, insn, "bltu");
	case funct3_bgeu:	return render_btype(addr, insn, "bgeu");
      }
      assert (0 && "unrecognized funct3"); //impossible
    //if opcode matches that of a load immediate, look at funct3 to differentiate
    case opcode_load_imm:
      switch (funct3)
      {
        default:		return render_illegal_insn(insn);
        case funct3_lb:		return render_itype_load(insn, "lb");
	case funct3_lh:		return render_itype_load(insn, "lh");
	case funct3_lw:		return render_itype_load(insn, "lw");
	case funct3_lbu:	return render_itype_load(insn, "lbu");
	case funct3_lhu:	return render_itype_load(insn, "lhu");
      }
      assert (0 && "unrecognized funct3"); //impossible
    //if opcode matches that of a stype, look at funct3 to differentiate
    case opcode_stype:
      switch (funct3)
      {
        default:		return render_illegal_insn(insn);
	case funct3_sb:		return render_stype(insn, "sb");
	case funct3_sh:		return render_stype(insn, "sh");
	case funct3_sw:		return render_stype(insn, "sw");
      }
      assert (0 && "unrecognized funct3"); //impossible
    //if the opcode matches that of a alu immediate, look at funct3 to differentiate
    case opcode_alu_imm:
      switch (funct3)
      {
	default:		return render_illegal_insn(insn);
	case funct3_add:	return render_itype_alu(insn, "addi", get_imm_i(insn));
	case funct3_slt:	return render_itype_alu(insn, "slti", get_imm_i(insn));
	case funct3_sltu:	return render_itype_alu(insn, "sltiu", get_imm_i(insn));
	case funct3_xor:	return render_itype_alu(insn, "xori", get_imm_i(insn));
	case funct3_or:		return render_itype_alu(insn, "ori", get_imm_i(insn));
	case funct3_and:	return render_itype_alu(insn, "andi", get_imm_i(insn));
	case funct3_sll:	return render_itype_alu(insn, "slli", get_imm_i(insn)%XLEN);
	//if the opcode matches that of a srx instruction, look at funct7 to differentiate
	case funct3_srx:
	  switch (funct7)
	  {
	    default:		return render_illegal_insn(insn);
	    case funct7_srl:	return render_itype_alu(insn, "srli", get_imm_i(insn)%XLEN);
	    case funct7_sra:	return render_itype_alu(insn, "srai", get_imm_i(insn)%XLEN);
	  }
	  assert(0 && "unrecognized funct7"); //impossible*/
      }
      assert (0 && "unrecognized funct3"); //impossible
    //if the opcode matches that of a rtype, look at funct3 to differentiate
    case opcode_rtype:
      switch (funct3)
      {
	default:		return render_illegal_insn(insn);
	//if the opode matches the add instruction, look at funct7 to differentiate
        case funct3_add:
	  switch (funct7)
	  {
	    default:		return render_illegal_insn(insn);
	    case funct7_add:	return render_rtype(insn, "add");
	    case funct7_sub:	return render_rtype(insn, "sub");
	  }
	  assert (0 && "unrecognized funct7"); //impossible
	case funct3_sll:	return render_rtype(insn, "sll");
	case funct3_slt:	return render_rtype(insn, "slt");
	case funct3_sltu:	return render_rtype(insn, "sltu");
	case funct3_xor:	return render_rtype(insn, "xor");
	//if the opcode matches that of a srx instruction, look at funct7 to differentiate
	case funct3_srx:
	  switch (funct7)
	  {
            default:		return render_illegal_insn(insn);
	    case funct7_srl:	return render_rtype(insn, "srl");
	    case funct7_sra:	return render_rtype(insn, "sra");
	  }
	  assert (0 && "unrecognized funct7"); //impossible
	case funct3_or:		return render_rtype(insn, "or");
	case funct3_and:	return render_rtype(insn, "and");
      }
      assert (0 && "unrecognized funct3"); //impossible
    //if the opcode matches that of a system instruction, look at funct3 to differentiate
    case opcode_system:
      switch (funct3)
      {
	default:		//return render_illegal_insn(insn);
	case funct3_csrrw:	return render_csrrx(insn, "csrrw");
	case funct3_csrrs:	return render_csrrx(insn, "csrrs");
	case funct3_csrrc:	return render_csrrx(insn, "csrrc");
	case funct3_csrrwi:	return render_csrrxi(insn, "csrrwi");
	case funct3_csrrsi:	return render_csrrxi(insn, "csrrsi");
	case funct3_csrrci:	return render_csrrxi(insn, "csrrci");
      }
      assert (0 && "unrecognized funct3"); //impossible
  }
  assert (0 && "unrecognized opcode"); //impossible
}

/**
 * @brief Is called when an illegal instruction is attempted to be rendered
 *
 * @param insn The instruction that was attempted to be rendered
 *
 * @return Error message that the string was unimplemented
 **********************************************************************************************/
std::string rv32i_decode::render_illegal_insn(uint32_t insn)
{
  return "ERROR: UNIMPLEMENTED INSTRUCTION";
}

/**
 * @brief Decodes the lui instruction
 *
 * @param insn The instruction that will be rendered
 *
 * @return Name of the instruction, the rd, and the imm_u value
 ***********************************************************************************************/
std::string rv32i_decode::render_lui(uint32_t insn)
{
  //variable definitios to get rd and imm_u
  uint32_t rd = get_rd(insn);
  int32_t imm_u = get_imm_u(insn);

  //set string to the decoded version of the instruction and return it
  std::ostringstream os;
  os << render_mnemonic("lui") << render_reg(rd) << "," << 
	  hex::to_hex0x20((imm_u >> 12) & 0x0fffff);
  return os.str();
}

/**
 * @brief Decodes the auipc instruction
 *
 * @param insn The instruction that will be rendered
 *
 * return Name of the instruction, the rd, and the imm_u value
 **********************************************************************************************/
std::string rv32i_decode::render_auipc(uint32_t insn)
{
  //variable definitions to get rd and imm_u
  uint32_t rd = get_rd(insn);
  int32_t imm_u = get_imm_u(insn);

  //set string to the decoded version of the instruction
  std::ostringstream os;
  os << render_mnemonic("auipc") << render_reg(rd) << "," << 
	  hex::to_hex0x20((imm_u >> 12) & 0x0fffff);
  return os.str();
}

/**
 * @brief Decodes the jal instruction
 *
 * @param addr Address of the instruction to decode
 * @param insn The instruction that will be rendered
 *
 * return Name of the instruction, the rd, and the pcrel_21 value
 **********************************************************************************************/
std::string rv32i_decode::render_jal(uint32_t addr, uint32_t insn)
{
  //variable definitions for rd, imm_j, and pcrel_21
  uint32_t rd = get_rd(insn);
  int32_t imm_j = get_imm_j(insn);
  int32_t pcrel_21 = addr + imm_j;

  //set string to the decoded version of the instruction and return it
  std::ostringstream os;
  os << render_mnemonic("jal") << render_reg(rd) << "," << 
	  hex::to_hex0x32(pcrel_21);
  return os.str();
}

/**
 * @brief Decodes the jalr instruction
 *
 * @param insn The instruction that will be rendered
 *
 * @return Name of the instruction, the rd, the imm_j value, and the rs2
 **********************************************************************************************/
std::string rv32i_decode::render_jalr(uint32_t insn)
{
  //varaible definitions for rd, rs1, and imm_i
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  int32_t imm_i = get_imm_i(insn);

  //set string to the decoded version of the instruction and return it
  std::ostringstream os;
  os << render_mnemonic("jalr") << render_reg(rd) << "," << 
	  render_base_disp(rs1, imm_i);
  return os.str();
}

/**
 * @brief Decodes all B type instructions
 *
 * @param addr Address of the instruction to decode
 * @param insn The instruction that will be rendered
 * @param mnemonic The name of the instruction that will be rendered
 *
 * @return The name of the instruction, the rs1, and rs2, and the pcrel_13 value
 **********************************************************************************************/
std::string rv32i_decode::render_btype(uint32_t addr, uint32_t insn, const char *mnemonic)
{
  //variable definitions for rs1, rs2, imm_b, and pcrel_13
  uint32_t rs1 = get_rs1(insn);
  uint32_t rs2 = get_rs2(insn);
  int32_t imm_b = get_imm_b(insn);
  int32_t pcrel_13 = addr + imm_b;

  //set string to the decoded version of the instruction and return it
  std::ostringstream os;
  os << render_mnemonic(mnemonic) << render_reg(rs1) << "," << 
	  render_reg(rs2) << "," << hex::to_hex0x32(pcrel_13);
  return os.str();
}

/**
 * @brief Decodes all of the I type load instructions
 *
 * @param insn the instruction that will be rendered
 * @param mnemonic The name of the instruction that will be rendered
 *
 * @return The name of the instruction, the rd, the imm_i value, and the rs1
 **********************************************************************************************/
 std::string rv32i_decode::render_itype_load(uint32_t insn, const char *mnemonic)
{
  //variable definition for rd, rs1, imm_i
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  int32_t imm_i = get_imm_i(insn);

  //set string to the decoded version of the instruction and return it
  std::ostringstream os;
  os << render_mnemonic(mnemonic) << render_reg(rd) << "," << 
	  render_base_disp(rs1, imm_i);
  return os.str();
}

/**
 * @brief Decodes all I type alu instructions
 *
 * @param insn The instruction that will be deoced
 * @param mnemonic The name of the instruction that will be decoded
 * @param imm_i The immediate value of the instruction
 *
 * @return The name of the instruction, the rd, the rs1, and the imm_i value
 **********************************************************************************************/
std::string rv32i_decode::render_itype_alu(uint32_t insn, const char *mnemonic, int32_t imm_i)
{
  //variable definition for rd and rs1
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  
  //set string to the decoded version of the instruction and return it
  std::ostringstream os;
  os << render_mnemonic(mnemonic) << render_reg(rd) << "," <<
	  render_reg(rs1) << "," << imm_i;
  return os.str();
}

/**
 * @brief Decodes all S type instructions
 *
 * @param insn The instruction that will be decoded
 * @param mnemonic The name of the instruction that will be decoded
 *
 * @return The name of the instruction, the rs2, the imm_s value, and the rs2
 **********************************************************************************************/
std::string rv32i_decode::render_stype(uint32_t insn, const char *mnemonic)
{
  //variable definitions for rs1, rs2, and imm_s
  uint32_t rs1 = get_rs1(insn);
  uint32_t rs2 = get_rs2(insn);
  int32_t imm_s = get_imm_s(insn);

  //set string to the decoded version of the instruction and return it
  std::ostringstream os;
  os << render_mnemonic(mnemonic) << render_reg(rs2) << "," << 
	  render_base_disp(rs1, imm_s);
  return os.str();
}

/**
 * @brief Decodes all R type instructions
 *
 * @param insn The instruction that will be decoded
 * @param mnemonic The name of the instruction that will be decoded
 *
 * @return The name of the instruction, the rd, the rs1, and the rs2
 **********************************************************************************************/
std::string rv32i_decode::render_rtype(uint32_t insn, const char *mnemonic)
{
  //variable definitions for rd, rs1, and rs2
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  uint32_t rs2 = get_rs2(insn);

  //set string to the decoded version of the instruction and return it
  std::ostringstream os;
  os << render_mnemonic(mnemonic) << render_reg(rd) << "," << 
	  render_reg(rs1) << "," << render_reg(rs2);
  return os.str();
}

/**
 * @brief Returns ecall when the ecall instruction is provided
 *
 * @param insn Instruction for ecall
 *
 * @return The string "ecall"
 **********************************************************************************************/
std::string rv32i_decode::render_ecall(uint32_t insn)
{
  //set the string to ecall and return it
  std::ostringstream os;
  os << "ecall";
  return os.str();
}

/**
 * @brief Returns ebreak when the ebreak instruction is provided
 *
 * @param insn Instruction for ebreak
 *
 * @return The string "ebreak"
 **********************************************************************************************/
std::string rv32i_decode::render_ebreak(uint32_t insn)
{
  //set the string to ebreak and return it
  std::ostringstream os;
  os << "ebreak";
  return os.str();
}

/**
 * @brief Decode all csrrx instructions
 *
 * @param insn the instruction that will be rendered
 * @param mnemonic The name of the instruction that will be rendered
 *
 * @return The name of the instructions, the rd, the csr value, and the rs1
 **********************************************************************************************/
std::string rv32i_decode::render_csrrx(uint32_t insn, const char *mnemonic)
{
  //variable definitions for rd, rs1, and csr
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  int32_t csr = get_imm_i(insn);

  //set string to decoded version of the instruction and return it
  std::ostringstream os;
  os << render_mnemonic(mnemonic) << render_reg(rd) << "," << 
	  hex::to_hex0x12(csr) << "," << render_reg(rs1);
  return os.str();
}

/**
 * @brief decode all csrrxi instructions
 *
 * @param insn The instruction that will be rendered
 * @param mnemonic The name of the instruction that will be rendered
 *
 * @return The name of the instruction, the rd, the csr value, and the zimm value
 **********************************************************************************************/
 std::string rv32i_decode::render_csrrxi(uint32_t insn, const char *mnemonic)
{
  //variable definitions for rd, csr, and zimm
  uint32_t rd = get_rd(insn);
  int32_t csr = get_imm_i(insn);
  uint32_t zimm = get_rs1(insn);

  //set string to decoded version of the instruction and return it
  std::ostringstream os;
  os << render_mnemonic(mnemonic) << render_reg(rd) << "," << 
	  hex::to_hex0x12(csr) << "," << zimm;
  return os.str();
}

/**
 * @brief Render the name of the register with the given number r
 *
 * @param r The name of the register
 *
 * @return The name of a register as a string
 **********************************************************************************************/
std::string rv32i_decode::render_reg(int r)
{
  //set string to "x" and then the register and return it
  std::ostringstream os;
  os << "x" << r;
  return os.str();
}

/**
 * @brief Render the operands of the form imm(register)
 *
 * @param register The register that will be rendered
 * @param imm The immediate value that will be rendered
 *
 * @return The operands in the form imm(register)
 **********************************************************************************************/
std::string rv32i_decode::render_base_disp(uint32_t r, int32_t imm)
{
  //set string to the register in paranthese with a x in front of it and return it
  std::ostringstream os;
  os << imm << "(" << render_reg(r) << ")";
  return os.str();
}

/**
 * @brief Render the menmonic with mnemoic_width space padding on the right
 *
 * @param mnemonic Mnemonic to be padded on the right with
 *
 * @return Return the right space padded mnemonic
 **********************************************************************************************/
std::string rv32i_decode::render_mnemonic(const std::string &mnemonic)
{
  //set string to the mnemonic with a set width filled with spaces and return it
  std::ostringstream os;
  os << std::setw(mnemonic_width) << std::setfill(' ') << 
	  std::left << mnemonic;
  return os.str();
}

/**
 * @brief Extract and return the opcode field from the given instruction
 *
 * @param insn Instruction to extract the opcode field from
 *
 * @return The last 7 bits of the instruction representing the opcode field
 **********************************************************************************************/
uint32_t rv32i_decode::get_opcode(uint32_t insn)
{
  return (insn & 0x0000007f);
}

/**
 * @brief Extract and return the rd field from the given instruction
 *
 * @param insn Instruction to extract the rd field from
 *
 * @return The bits in the range 7-11 of the instruction representing the rd field
 **********************************************************************************************/
uint32_t rv32i_decode::get_rd(uint32_t insn)
{
  return (insn & 0x00000f80) >> 7;
}

/**
 * @brief Extract and return the funct3 field from the given instruction
 *
 * @param insn Instruction to extract the funct3 field from
 *
 * @return The bits in the range 12-14 of the instruction representing the funct3 field
 **********************************************************************************************/
uint32_t rv32i_decode::get_funct3(uint32_t insn)
{
  return (insn & 0x00007000) >> 12;
}

/**
 * @brief Extract and return the rs1 field from the given instruction
 *
 * @param insn Instruction to extract the rs1 field from
 *
 * @return The bits in the range 15-19 of the instruction representing the rs1 field
 **********************************************************************************************/
uint32_t rv32i_decode::get_rs1(uint32_t insn)
{
  return (insn & 0x000f8000) >> 15;
}

/**
 * @brief Extract and return the rs2 field from the given instruction
 *
 * @param insn Instruction to extract the rs2 field from
 *
 * @return The bits in the range 20-24 of the instruction representing the rs2 field
 **********************************************************************************************/
uint32_t rv32i_decode::get_rs2(uint32_t insn)
{
  return (insn & 0x01f00000) >> 20;
}

/**
 * @brief Extract and return the funct7 field from the given instruction
 *
 * @param insn Instruction to extract the funct7 field from
 *
 * @return The bits in the range 25-31 of the instruction representing the funct7 field
 **********************************************************************************************/
uint32_t rv32i_decode::get_funct7(uint32_t insn)
{
  return (insn & 0xfe000000) >> 25;
}

/**
 * @brief Extract and return tne imm_i field from the given instruction
 *
 * @param insn Instruction to extract the imm_i field from
 *
 * @return The bits of the instruction representing the imm_i field
 **********************************************************************************************/
int32_t rv32i_decode::get_imm_i(uint32_t insn)
{
  //shift all of the bits into place for the immediate
  int32_t imm_i = (insn & 0xfff00000) >> (20 - 0);	//right shift the selected bits from place 20 to 0

  //If the msb is on, sign-extend it
  if (insn & 0x80000000) { imm_i |= 0xfffff000; }

  return imm_i;
}

/**
 * @brief Extract and return the imm_u field from the given instruction
 *
 * @param insn Instruction to extract the imm_u field from
 *
 * @return The bits of the instruction representing the imm_u field
 **********************************************************************************************/
int32_t rv32i_decode::get_imm_u(uint32_t insn)
{
  //Shift all of the bit into place for the immediate
  int32_t imm_u = (insn & 0x00000fff) & 0;	//Bits 0-11 are 0
  imm_u |= (insn & 0xfffff000);			//The rest of the bits remain as they were

  return imm_u;
}

/**
 * @brief Extract and return the imm_b field from the given instruction
 *
 * @param insn Instruction to extract the imm_b field from
 *
 * @return The bits of the instruction representing the imm_b field
 **********************************************************************************************/
int32_t rv32i_decode::get_imm_b(uint32_t insn)
{
  //Shift all of the bits into place for the immediate
  int32_t imm_b = (insn & 0x00000001) & 0;	//the lsb bit is always 0
  imm_b |= (insn & 0x00000f00) >> (8 - 1);	//right shift the selected bits from place 8 to 1
  imm_b |= (insn & 0x7e000000) >> (25 - 5);	//right shift the selected bits from place 25 to 5
  imm_b |= (insn & 0x00000080) << (11 - 7);	//left shift the selected bit from place 7 to 11
  imm_b |= (insn & 0x80000000) >> (31 - 12);	//right shift the msb bit from place 31 to 12

  //If the msb in on, sign-extend it
  if (insn & 0x80000000) { imm_b |= 0xffffe000; }

  return imm_b;
}

/**
 * @brief Extract and return the imm_s field from the given instruction
 *
 * @param insn Instruction to extract the imm_s field from
 *
 * @return The bits of the instruction representing the imm_s field
 **********************************************************************************************/
int32_t rv32i_decode::get_imm_s(uint32_t insn)
{
  //shift all of the bits into place for the immediate
  int32_t imm_s = (insn & 0xfe000000) >> (25 - 5);	//right shift the selected bits from place 25 to 5
  imm_s |= (insn & 0x00000f80) >> (7 - 0);		//right shift the selected bits from place 7 to 0

  //if the msb is on, sign-extend left
  if (insn & 0x80000000) { imm_s |= 0xfffff000; }

  return imm_s;
}

/**
 * @brief Extract and return the imm_j field from the given instruction
 *
 * @param insn Instruction to exract the imm_j field from
 *
 * @return The bits of the instruction representing the imm_j field
 **********************************************************************************************/
int32_t rv32i_decode::get_imm_j(uint32_t insn)
{
  //shift all of the bits into place for the immediate
  int32_t imm_j = (insn & 0x00000001) & 0;		//the lsb bit is always 0
  imm_j |= (insn & 0x7fe00000) >> (21 - 1);		//right shift the selected bits from place 21 to 1
  imm_j |= (insn & 0x00100000) >> (20 - 11);		//right shift the selected bit from place 20 to 11
  imm_j |= (insn & 0x000ff000);				//Bits 12-19 remain as they were
  imm_j |= (insn & 0x80000000) >> (31 - 20);		//right shift the selected bit from place 31 to 20

  //if the msb is on, sign-extend left
  if (insn & 0x80000000) { imm_j |= 0xffe00000; }

  return imm_j;
}
