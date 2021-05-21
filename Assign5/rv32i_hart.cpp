//***************************************************************************
//
//  Alex Marine (z1863905)
//  Assignment 5
//  CSCI 463-1
//  3/23/2021
//
//  I certify that this is my own work and where appropriate an extension
//  of the starter code provided for the assignment
//
//***************************************************************************

#include "rv32i_hart.h"

/**
 * @brief Tells the simulator to execute an instruction
 *
 * @param hdr Printed on the left of any and all outputs that are displayed from this method
 ***********************************************************************************************/
void rv32i_hart::tick(const std::string &hdr)
{
  //if halt is true, leave the method
  if (halt == true)
  {
    return;
  }

  //increment the instruction counter
  insn_counter += 1;

  //if show_registers is true, then dump the state of the hart
  if (show_registers == true)
  {
    dump(hdr);
  }

  //fetch an instruction from the memory at the address in the pc register
  uint32_t insn = mem.get32(pc);

  //if show_instruction is true then..
  if (show_instructions == true)
  {
    //output the hdr, pc register, and fetched instruction
    std::cout << hdr << hex::to_hex32(pc) << ": " << hex::to_hex32(insn) << "  ";

    //call exec to execute the instruction and render the instruction and simulation details
    exec(insn, &std::cout);
    std::cout << std::endl;
  }
  //else, call exec to execute the instruction without rendering anything
  else
  {
    exec(insn, nullptr);
  }
}

/**
 * @brief Dump the entire state of the hart
 *
 * @param hdr Prefix each line with hdr if it is provided
 ***********************************************************************************************/
void rv32i_hart::dump(const std::string &hdr) const
{
  regs.dump(hdr);
  std::cout << std::endl << std::right << std::setw(4) << "pc " << hex::to_hex32(pc) << std::endl;
}

/**
 * @brief Reset the rv32i object and the registerfile
 ***********************************************************************************************/
void rv32i_hart::reset()
{
  //set the pc register to zero
  pc = 0;
  
  //call regs.reset to reset the register values
  regs.reset();

  //set the insn_counter to zero
  insn_counter = 0;

  //set the halt falg to false
  halt = false;
  
  //set the halt_reason to "none"
  halt_reason = "none";
}

/**
 * @brief Execute the given RV32I instruction by using a switch
 *
 * @param insn The instruction that will be executed
 * @param pos Output stream that will be written to
 ***********************************************************************************************/
void rv32i_hart::exec(uint32_t insn, std::ostream* pos)
{
  //variable definitions to get funct3 and funct7 for instructions
  uint32_t funct3 = get_funct3(insn);
  uint32_t funct7 = get_funct7(insn);

  //if the insn matches ebreak's, call ebreaks's exec function
  if (insn == insn_ebreak) { exec_ebreak(insn, pos); return; }

  switch(get_opcode(insn))
  {
  default:		exec_illegal_insn(insn, pos); return;
  case opcode_lui:	exec_lui(insn, pos); return;
  case opcode_auipc:	exec_auipc(insn, pos); return;
  case opcode_jal:	exec_jal(insn, pos); return;
  case opcode_jalr:	exec_jalr(insn, pos); return;
  //if opcode matches that of a btype, look at funct3 to differentiate
  case opcode_btype:
    switch (funct3)
    {
    default:		exec_illegal_insn(insn, pos); return;
    case funct3_beq:	exec_beq(insn, pos); return;
    case funct3_bne:	exec_bne(insn, pos); return;
    case funct3_blt:	exec_blt(insn, pos); return;
    case funct3_bge:	exec_bge(insn, pos); return;
    case funct3_bltu:	exec_bltu(insn, pos); return;
    case funct3_bgeu:	exec_bgeu(insn, pos); return;
    }
    assert (0 && "unrecognized funct3");
  //if opcode matches that of a load immediate, look at funct3 to differentiate
  case opcode_load_imm:
    switch (funct3)
    {
    default:		exec_illegal_insn(insn, pos); return;
    case funct3_lb:	exec_lb(insn, pos); return;
    case funct3_lh:	exec_lh(insn, pos); return;
    case funct3_lw:	exec_lw(insn, pos); return;
    case funct3_lbu:	exec_lbu(insn, pos); return;
    case funct3_lhu:	exec_lhu(insn, pos); return;
    }
    assert (0 && "unrecognized funct3");
  //if opcode matches that of a stype, look at funct3 to differentiate
  case opcode_stype:
    switch (funct3)
    {
    default:		exec_illegal_insn(insn, pos); return;
    case funct3_sb:	exec_sb(insn, pos); return;
    case funct3_sh:	exec_sh(insn, pos); return;
    case funct3_sw:	exec_sw(insn, pos); return;
    }
    assert (0 && "unrecognized funct3");
  case opcode_alu_imm:
    switch (funct3)
    {
    default:		exec_illegal_insn(insn, pos); return;
    case funct3_add:	exec_addi(insn, pos); return;
    case funct3_slt:	exec_slti(insn, pos); return;
    case funct3_sltu:	exec_sltiu(insn, pos); return;
    case funct3_xor:	exec_xori(insn, pos); return;
    case funct3_or:	exec_ori(insn, pos); return;
    case funct3_and:	exec_andi(insn, pos); return;
    case funct3_sll:	exec_slli(insn, pos); return;
    //if the opcode matches that of a srx instruction, look at funct7 to differentiate
    case funct3_srx:
      switch (funct7)

      {
      default:		exec_illegal_insn(insn, pos); return;
      case funct7_srl:	exec_srli(insn, pos); return;
      case funct7_sra:	exec_srai(insn, pos); return;
      }
      assert (0 && "unrecognized funct7");
    }
    assert (0 && "unrecognized funct3");
  //if the opcode matches that of a rtype, look at funct3 to differentiate
  case opcode_rtype:
    switch (funct3)
    {
    default:		exec_illegal_insn(insn, pos); return;
    //if the opcode matches that of add, look at funct7 to differentiate
    case funct3_add:
      switch (funct7)
      {
      default:		exec_illegal_insn(insn, pos); return;
      case funct7_add:	exec_add(insn, pos); return;
      case funct7_sub:	exec_sub(insn, pos); return;
      }
      assert (0 && "unrecognozed funct7");
    case funct3_sll:	exec_sll(insn, pos); return;
    case funct3_slt:	exec_slt(insn, pos); return;
    case funct3_sltu:	exec_sltu(insn, pos); return;
    case funct3_xor:	exec_xor(insn, pos); return;
    //if the opcode matches that of a srx instruction, look at funct7 to differentiate
    case funct3_srx:
      switch (funct7)
      {
      default:		exec_illegal_insn(insn, pos); return;
      case funct7_srl:	exec_srl(insn, pos); return;
      case funct7_sra:	exec_sra(insn, pos); return;
      }
      assert (0 && "unrecognized funct7");
    case funct3_or:	exec_or(insn, pos); return;
    case funct3_and:	exec_and(insn, pos); return;
    }
    assert (0 && "unrecognized funct3");
  //if the opcode matches that of a system instruction, look at funct3 to differentiate
  case opcode_system:
    switch (funct3)
    {
    default:		exec_illegal_insn(insn, pos); return;
    case funct3_csrrs:	exec_csrrs(insn, pos); return;
    }
    assert (0 && "unrecognized funct3");
  }
  assert (0 && "unrecognized opcode");
}

/**
 * @brief Executes an illegal instruction
 *
 * @param insn The instruction that will be executed
 * @param pos Output stream that will be written to
 *
 * @note Code given in assignment handout
 ***********************************************************************************************/
void rv32i_hart::exec_illegal_insn(uint32_t insn, std::ostream* pos)
{
  (void)insn;
  if (pos) { *pos << render_illegal_insn(insn); }
  halt = true;
  halt_reason = "Illegal instruction";
}

/**
 * @brief Executes the lui instruction
 *
 * @param insn The instruction that will be executed
 * @param pos Output stream that will be written to
 ***********************************************************************************************/
void rv32i_hart::exec_lui(uint32_t insn, std::ostream* pos)
{
  //variable definitions to get rd and imm_u
  uint32_t rd = get_rd(insn);
  uint32_t imm_u = get_imm_u(insn);

  if (pos)
  {
    //output the detailed description of the instruction
    std::string s = render_lui(insn);
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(imm_u);
  }

  //set the register to the immediate and increment the value
  regs.set(rd, imm_u);
  pc += 4;
}

/**
 * @brief Executes the auipic instruction
 *
 * @param insn The instruction that will be executed
 * @param pos Output stream that will be written to
 ***********************************************************************************************/
void rv32i_hart::exec_auipc(uint32_t insn, std::ostream* pos)
{
  //variable definitions to get rd and imm_u
  uint32_t rd = get_rd(insn);
  uint32_t imm_u = get_imm_u(insn);

  //variable to hold the value stored at rd
  int32_t val = pc + imm_u;

  if (pos)
  {
    //output the detailed description of the instruction
    std::string s = render_auipc(insn);
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(pc) << " + " << 
	    hex::to_hex0x32(imm_u) << " = " << hex::to_hex0x32(val);
  }

  //set the register to the value and increment pc
  regs.set(rd, val);
  pc += 4;
}

/**
 * @brief Executes the jal instruction
 *
 * @param insn The instruction that will be executed
 * @param pos Output stream that will be written to
 ***********************************************************************************************/
void rv32i_hart::exec_jal(uint32_t insn, std::ostream* pos)
{
  //variable definitons to get rd and imm_j
  uint32_t rd = get_rd(insn);
  uint32_t imm_j = get_imm_j(insn);

  //variable to hold value stored at rd and pc
  int32_t val_rd = pc + 4;
  int32_t val_pc = pc + imm_j;

  if (pos)
  {
    //output the detailed description of the instruction
    std::string s = render_jal(pc, insn);
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(val_rd) << ",  pc = " << 
	    hex::to_hex0x32(pc) << " + " << hex::to_hex0x32(imm_j) << " = " << hex::to_hex0x32(val_pc);
  }

  //set the register to the value and set pc to val_pc
  regs.set(rd, val_rd);
  pc = val_pc;

}

/**
 * @brief Executes the jalr instruction
 *
 * @param insn The instruction that will be executed
 * @param pos Output stream that will be written to
 ***********************************************************************************************/
void rv32i_hart::exec_jalr(uint32_t insn, std::ostream* pos)
{
  //variable definitions to get rd, rs1, imm_i
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  uint32_t imm_i = get_imm_i(insn);

  //variable to hold the value stored at rd and pc
  int32_t val_rd = pc + 4;
  int32_t val_pc = (regs.get(rs1) + imm_i) & 0xfffffffe;

  if (pos)
  {
    //output the detailed description to the instruction
    std::string s = render_jalr(insn);
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(val_rd) << ",  pc = (" << hex::to_hex0x32(imm_i) <<
	    " + " << hex::to_hex0x32(regs.get(rs1)) << ") & 0xfffffffe = " << hex::to_hex0x32(val_pc);
  }

  //set the register to the value and set pc to val_pc
  regs.set(rd, val_rd);
  pc = val_pc;

}

/**
 * @brief Executes the beq instruction
 *
 * @param insn The instruction that will be executed
 * @param pos Output stream that will be written to
 ***********************************************************************************************/
void rv32i_hart::exec_beq(uint32_t insn, std::ostream* pos)
{
  //variable definitions to get rs1, rs2, and imm_b
  uint32_t rs1 = get_rs1(insn);
  uint32_t rs2 = get_rs2(insn);
  uint32_t imm_b = get_imm_b(insn);

  //variable to hold the value stored at pc
  int32_t val = pc + ((regs.get(rs1) == regs.get(rs2)) ? imm_b : 4);

  if (pos)
  {
    //output the detailed description of the instruction
    std::string s = render_btype(pc, insn, "beq");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// pc += (" << hex::to_hex0x32(regs.get(rs1)) << " == " << hex::to_hex0x32(regs.get(rs2)) << 
	    " ? " << hex::to_hex0x32(imm_b) << " : 4) = " << hex::to_hex0x32(val);
  }

  //set the pc to val
  pc = val;
}

/**
 * @brief Executes the bne instruction
 *
 * @param insn The instruction that will be executed
 * @param pos Output stream that will be written to
 ***********************************************************************************************/
void rv32i_hart::exec_bne(uint32_t insn, std::ostream* pos)
{
  //variable definitions to get rs1, rs2, and imm_b
  uint32_t rs1 = get_rs1(insn);
  uint32_t rs2 = get_rs2(insn);
  uint32_t imm_b = get_imm_b(insn);

  //variable to hold the value stored at pc
  int32_t val = pc + ((regs.get(rs1) != regs.get(rs2)) ? imm_b : 4);

  if (pos)
  {
    //output the detailed description of the instruction
    std::string s = render_btype(pc, insn, "bne");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// pc += (" << hex::to_hex0x32(regs.get(rs1)) << " != " << hex::to_hex0x32(regs.get(rs2)) << 
	    " ? " << hex::to_hex0x32(imm_b) << " : 4) = " << hex::to_hex0x32(val);
  }

  //set the pc to val
  pc = val;
}

/**
 * @brief Executes the blt instruction
 *
 * @param insn The instruction that will be executed
 * @param pos Output stream that will be written to
 ***********************************************************************************************/
void rv32i_hart::exec_blt(uint32_t insn, std::ostream* pos)
{
  //variable definitions to get rs1, rs2, and imm_b
  uint32_t rs1 = get_rs1(insn);
  uint32_t rs2 = get_rs2(insn);
  uint32_t imm_b = get_imm_b(insn);

  //variable to hold the value stored at pc
  int32_t val = pc + ((regs.get(rs1) < regs.get(rs2)) ? imm_b : 4);

  if (pos)
  {
    //output the detailed description of the instruction
    std::string s = render_btype(pc, insn, "blt");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// pc += (" << hex::to_hex0x32(regs.get(rs1)) << " < " << hex::to_hex0x32(regs.get(rs2)) << 
	    " ? " << hex::to_hex0x32(imm_b) << " : 4) = " << hex::to_hex0x32(val);
  }

  //set the pc to val
  pc = val;
}

/**
 * @brief Executes the bge instruction
 *
 * @param insn The instruction that will be executed
 * @param pos Output stream that will be written to
 ***********************************************************************************************/
void rv32i_hart::exec_bge(uint32_t insn, std::ostream* pos)
{
  //variable definitions to get rs1, rs2, and imm_b
  uint32_t rs1 = get_rs1(insn);
  uint32_t rs2 = get_rs2(insn);
  uint32_t imm_b = get_imm_b(insn);

  //variable to hold the value stored at pc
  int32_t val = pc + ((regs.get(rs1) >= regs.get(rs2)) ? imm_b : 4);

  if (pos)
  {
    //output the detailed description of the instruction
    std::string s = render_btype(pc, insn, "bge");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// pc += (" << hex::to_hex0x32(regs.get(rs1)) << " >= " << hex::to_hex0x32(regs.get(rs2)) << 
	    " ? " << hex::to_hex0x32(imm_b) << " : 4) = " << hex::to_hex0x32(val);
  }

  //set the pc to val
  pc = val;
}

/**
 * @brief Executes the bltu instruction
 *
 * @param insn The instruction that will be executed
 * @param pos Output stream that will be written to
 ***********************************************************************************************/
void rv32i_hart::exec_bltu(uint32_t insn, std::ostream* pos)
{
  //variable definitions to get rs1, rs2, and imm_b
  uint32_t rs1 = get_rs1(insn);
  uint32_t rs2 = get_rs2(insn);
  uint32_t imm_b = get_imm_b(insn);

  //variable to hold the value stored at pc
  uint32_t val = pc + ((static_cast<uint32_t>(regs.get(rs1)) < static_cast<uint32_t>(regs.get(rs2))) ? imm_b : 4);

  if (pos)
  {
    //output the detailed description of the instruction
    std::string s = render_btype(pc, insn, "bltu");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// pc += (" << hex::to_hex0x32(regs.get(rs1)) << " <U " << hex::to_hex0x32(regs.get(rs2)) << 
	    " ? " << hex::to_hex0x32(imm_b) << " : 4) = " << hex::to_hex0x32(val);
  }

  //set the pc to val
  pc = val;
}

/**
 * @brief Executes the bgeu instruction
 *
 * @param insn The instruction that will be executed
 * @param pos Output stream that will be written to
 ***********************************************************************************************/
void rv32i_hart::exec_bgeu(uint32_t insn, std::ostream* pos)
{
  //variable definitions to get rs1, rs2, and imm_b
  uint32_t rs1 = get_rs1(insn);
  uint32_t rs2 = get_rs2(insn);
  uint32_t imm_b = get_imm_b(insn);

  //variable to hold the value stored at pc
  uint32_t val = pc + ((static_cast<uint32_t>(regs.get(rs1)) >= static_cast<uint32_t>(regs.get(rs2))) ? imm_b : 4);

  if (pos)
  {
    //output the detailed description of the instruction
    std::string s = render_btype(pc, insn, "bgeu");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// pc += (" << hex::to_hex0x32(regs.get(rs1)) << " >=U " << hex::to_hex0x32(regs.get(rs2)) << 
	    " ? " << hex::to_hex0x32(imm_b) << " : 4) = " << hex::to_hex0x32(val);
  }

  //set the pc to val
  pc = val;
}

/**
 * @brief Executes the lb instruction
 *
 * @param insn The instruction that will be executed
 * @param pos Output stream that will be written to
 ***********************************************************************************************/
void rv32i_hart::exec_lb(uint32_t insn, std::ostream* pos)
{
  //variable definitions to get rd, rs1, and imm_i
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  uint32_t imm_i = get_imm_i(insn);

  //variable to hold address that we need and the value at that address
  int32_t addr = regs.get(rs1) + imm_i;
  int32_t val = mem.get8(addr);

  //if msb is on, sign-extend val to the left
  if (val & 0x00000080) { val |= 0xffffff00; }

  if (pos)
  {
    //output the detailed description of the instruction
    std::string s = render_itype_load(insn, "lb");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = sx(m8(" << hex::to_hex0x32(regs.get(rs1)) << " + " <<
	    hex::to_hex0x32(imm_i) << ")) = " << hex::to_hex0x32(val);
  }

  //set the register to the value and set pc to val_pc
  regs.set(rd, val);
  pc += 4;
}

/**
 * @brief Executes the lh instruction
 *
 * @param insn The instruction that will be executed
 * @param pos Output stream that will be written to
 ***********************************************************************************************/
void rv32i_hart::exec_lh(uint32_t insn, std::ostream* pos)
{
  //variable definitions to get rd, rs1, and imm_i
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  uint32_t imm_i = get_imm_i(insn);

  //variables to hold address that we need and the value at that address
  int32_t addr = regs.get(rs1) + imm_i;
  int32_t val = mem.get16(addr);

  //if msb is on, sign-extend it to the left
  if (val & 0x00008000) { val |= 0xffff0000; }

  if (pos)
  {
    //output the detailed description of the instruction
    std::string s = render_itype_load(insn, "lh");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = sx(m16(" << hex::to_hex0x32(regs.get(rs1)) << " + " <<
	    hex::to_hex0x32(imm_i) << ")) = " << hex::to_hex0x32(val);
  }

  //set the register to the value and set pc to val_pc
  regs.set(rd, val);
  pc += 4;
}

/**
 * @brief Executes the lw instruction
 *
 * @param insn The instruction that will be executed
 * @param pos Output stream that will be written to
 ***********************************************************************************************/
void rv32i_hart::exec_lw(uint32_t insn, std::ostream* pos)
{
  //variable definitions to get rd, rs1, and imm_i
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  uint32_t imm_i = get_imm_i(insn);

  //variables to hold address that we need and the value at that address
  int32_t addr = regs.get(rs1) + imm_i;
  int32_t val = mem.get32(addr);

  //if msb is on, sign-extend it to the left
  if (val & 0x80000000) { val |= 0x00000000; }

  if (pos)
  {
    //output the detailed description of the instruction
    std::string s = render_itype_load(insn, "lw");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = sx(m32(" << hex::to_hex0x32(regs.get(rs1)) << " + " <<
	    hex::to_hex0x32(imm_i) << ")) = " << hex::to_hex0x32(val);
  }

  //set the register to the value and set pc to val_pc
  regs.set(rd, val);
  pc += 4;
}

/**
 * @brief Executes the lbu instruction
 *
 * @param insn The instruction that will be executed
 * @param pos Output stream that will be written to
 ***********************************************************************************************/
void rv32i_hart::exec_lbu(uint32_t insn, std::ostream* pos)
{
  //variable definitions to get rd, rs1, and imm_i
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  uint32_t imm_i = get_imm_i(insn);

  //variables to hold address that we need and the value at that address
  int32_t addr = regs.get(rs1) + imm_i;
  uint32_t val = mem.get8(addr);

  if (pos)
  {
    //output the detailed description of the instruction
    std::string s = render_itype_load(insn, "lbu");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = zx(m8(" << hex::to_hex0x32(regs.get(rs1)) << " + " <<
	    hex::to_hex0x32(imm_i) << ")) = " << hex::to_hex0x32(val);
  }

  //set the register to the value and set pc to val_pc
  regs.set(rd, val);
  pc += 4;
}

/**
 * @brief Executes the lhu instruction
 *
 * @param insn The instruction that will be executed
 * @param pos Output stream that will be written to
 ***********************************************************************************************/
void rv32i_hart::exec_lhu(uint32_t insn, std::ostream* pos)
{
  //variable definitions to get rd, rs1, and imm_i
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  uint32_t imm_i = get_imm_i(insn);

  //variables to hold address that we need and the value at that address
  int32_t addr = regs.get(rs1) + imm_i;
  uint32_t val = mem.get16(addr);

  if (pos)
  {
    //output the detailed description of the instruction
    std::string s = render_itype_load(insn, "lhu");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = zx(m16(" << hex::to_hex0x32(regs.get(rs1)) << " + " <<
	    hex::to_hex0x32(imm_i) << ")) = " << hex::to_hex0x32(val);
  }

  //set the register to the value and set pc to val_pc
  regs.set(rd, val);
  pc += 4;
}

/**
 * @brief Executes the sb instruction
 *
 * @param insn The instruction that will be executed
 * @param pos Output stream that will be written to
 ***********************************************************************************************/
void rv32i_hart::exec_sb(uint32_t insn, std::ostream* pos)
{
  //variable definition to get rs1, rs2, and imm_s
  uint32_t rs1 = get_rs1(insn);
  uint32_t rs2 = get_rs2(insn);
  uint32_t imm_s = get_imm_s(insn);

  //varianle to hold the address that we need and the value that will be stored at that address
  int32_t addr = regs.get(rs1) + imm_s;
  int32_t val = regs.get(rs2) & 0x000000ff;

  if (pos)
  {
    //output the detailed description of the instruction
    std::string s = render_stype(insn, "sb");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// m8(" << hex::to_hex0x32(regs.get(rs1)) << " + " << hex::to_hex0x32(imm_s) << 
	    ") = " << hex::to_hex0x32(val);
  }

  //set the address to val and increment the pc
  mem.set8(addr, val);
  pc += 4;
}

/**
 * @brief Executes the sh instruction
 *
 * @param insn The instruction that will be executed
 * @param pos Output stream that will be written to
 ***********************************************************************************************/
void rv32i_hart::exec_sh(uint32_t insn, std::ostream* pos)
{
  //variable definition to get rs1, rs2, and imm_s
  uint32_t rs1 = get_rs1(insn);
  uint32_t rs2 = get_rs2(insn);
  uint32_t imm_s = get_imm_s(insn);

  //varianle to hold the address that we need and the value that will be stored at that address
  int32_t addr = regs.get(rs1) + imm_s;
  int32_t val = regs.get(rs2) & 0x0000ffff;

  if (pos)
  {
    //output the detailed description of the instruction
    std::string s = render_stype(insn, "sh");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// m16(" << hex::to_hex0x32(regs.get(rs1)) << " + " << hex::to_hex0x32(imm_s) << 
	    ") = " << hex::to_hex0x32(val);
  }

  //set the address to val and increment the pc
  mem.set16(addr, val);
  pc += 4;
}

/**
 * @brief Executes the sw instruction
 *
 * @param insn The instruction that will be executed
 * @param pos Output stream that will be written to
 ***********************************************************************************************/
void rv32i_hart::exec_sw(uint32_t insn, std::ostream* pos)
{
  //variable definition to get rs1, rs2, and imm_s
  uint32_t rs1 = get_rs1(insn);
  uint32_t rs2 = get_rs2(insn);
  uint32_t imm_s = get_imm_s(insn);

  //varianle to hold the address that we need and that value that will be stored at that address
  int32_t addr = regs.get(rs1) + imm_s;
  int32_t val = regs.get(rs2);

  if (pos)
  {
    //output the detailed description of the instruction
    std::string s = render_stype(insn, "sw");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// m32(" << hex::to_hex0x32(regs.get(rs1)) << " + " << hex::to_hex0x32(imm_s) << 
	    ") = " << hex::to_hex0x32(val);
  }

  //set the address to val and increment the pc
  mem.set32(addr, val);
  pc += 4;
}

/**
 * @brief Executes the addi instruction
 *
 * @param insn The instruction that will be executed
 * @param pos Output stream that will be written to
 ***********************************************************************************************/
void rv32i_hart::exec_addi(uint32_t insn, std::ostream* pos)
{
  //variable definitions to get rd, rs1, and imm_i
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  uint32_t imm_i = get_imm_i(insn);

  //variable to hold the value stored at rd
  int32_t val = regs.get(rs1) + imm_i;

  if (pos)
  {
    //output the detailed description of the instruction
    std::string s = render_itype_alu(insn, "addi", imm_i);
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " + " << 
	    hex::to_hex0x32(imm_i) << " = " << hex::to_hex0x32(val);
  }

  //set the register to the value and increment the pc
  regs.set(rd, val);
  pc += 4;

}

/**
 * @brief Executes the slti instruction
 *
 * @param insn The instruction that will be executed
 * @param pos Output stream that will be written to
 ***********************************************************************************************/
void rv32i_hart::exec_slti(uint32_t insn, std::ostream* pos)
{
  //variable definitions to get rd, rs1, and imm_i
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  int32_t imm_i = get_imm_i(insn);

  //variable to hold the value stored at rd
  int32_t val = (regs.get(rs1) < imm_i) ? 1 : 0;

  if (pos)
  {
    //output the detailed description of the instruction
    std::string s = render_itype_alu(insn, "slti", imm_i);
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = (" << hex::to_hex0x32(regs.get(rs1)) << " < " <<
	    std::dec << imm_i << ") ? 1 : 0 = " << hex::to_hex0x32(val);
  }

  //set the register to the value and increment the pc
  regs.set(rd, val);
  pc += 4;
}

/**
 * @brief Executes the sltiu instruction
 *
 * @param insn The instruction that will be executed
 * @param pos Output stream that will be written to
 ***********************************************************************************************/
void rv32i_hart::exec_sltiu(uint32_t insn, std::ostream* pos)
{
  //variable definitions to get rd, rs1, and imm_i
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  int32_t imm_i = get_imm_i(insn);

  //variable to hold the value stored at rd
  uint32_t val = (static_cast<uint32_t>(regs.get(rs1)) < static_cast<uint32_t>(imm_i)) ? 1 : 0;

  if (pos)
  {
    //output the detailed description of the instruction
    std::string s = render_itype_alu(insn, "sltiu", imm_i);
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = (" << hex::to_hex0x32(regs.get(rs1)) << " <U " <<
	    std::dec << imm_i << ") ? 1 : 0 = " << hex::to_hex0x32(val);
  }

  //set the register to the value and increment the pc
  regs.set(rd, val);
  pc += 4;
}

/**
 * @brief Executes the xori instruction
 *
 * @param insn The instruction that will be executed
 * @param pos Output stream that will be written to
 ***********************************************************************************************/
void rv32i_hart::exec_xori(uint32_t insn, std::ostream* pos)
{
  //variable definitions to get rd, rs1, and imm_i
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  int32_t imm_i = get_imm_i(insn);

  //variable to hold the value stored at rd
  int32_t val = regs.get(rs1) ^ imm_i;

  if (pos)
  {
    //output the detailed description of the instruction
    std::string s = render_itype_alu(insn, "xori", imm_i);
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " ^ " <<
	    hex::to_hex0x32(imm_i) << " = " << hex::to_hex0x32(val);
  }

  //set the register to the value and increment the pc
  regs.set(rd, val);
  pc += 4; 
}

/**
 * @brief Executes the ori instruction
 *
 * @param insn The instruction that will be executed
 * @param pos Output stream that will be written to
 ***********************************************************************************************/
void rv32i_hart::exec_ori(uint32_t insn, std::ostream* pos)
{
  //variable definitions to get rd, rs1, and imm_i
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  int32_t imm_i = get_imm_i(insn);

  //variable to hold the value stored at rd
  int32_t val = regs.get(rs1) | imm_i;

  if (pos)
  {
    //output the detailed description of the instruction
    std::string s = render_itype_alu(insn, "ori", imm_i);
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " | " <<
	    hex::to_hex0x32(imm_i) << " = " << hex::to_hex0x32(val);
  }

  //set the register to the value and increment the pc
  regs.set(rd, val);
  pc += 4;
}

/**
 * @brief Executes the andi instruction
 *
 * @param insn The instruction that will be executed
 * @param pos Output stream that will be written to
 ***********************************************************************************************/
void rv32i_hart::exec_andi(uint32_t insn, std::ostream* pos)
{
  //variable definitions to get rd, rs1, and imm_i
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  int32_t imm_i = get_imm_i(insn);

  //variable to hold the value stored at rd
  int32_t val = regs.get(rs1) & imm_i;

  if (pos)
  {
    //output the detailed description of the instruction
    std::string s = render_itype_alu(insn, "andi", imm_i);
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " & " <<
	    hex::to_hex0x32(imm_i) << " = " << hex::to_hex0x32(val);
  }

  //set the register to the value and increment the pc
  regs.set(rd, val);
  pc += 4;
}

/**
 * @brief Executes the slli instruction
 *
 * @param insn The instruction that will be executed
 * @param pos Output stream that will be written to
 ***********************************************************************************************/
void rv32i_hart::exec_slli(uint32_t insn, std::ostream* pos)
{
  //variable definitions to get rd, rs1, and shamt_i
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  int32_t shamt_i = get_rs2(insn);

  //variable to hold the value stored at rd
  int32_t val = regs.get(rs1) << shamt_i;

  if (pos)
  {
    //output the detailed description of the instruction
    std::string s = render_itype_alu(insn, "slli", shamt_i);
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " << " <<
	    std::dec << shamt_i << " = " << hex::to_hex0x32(val);
  }

  //set the register to the value and increment the pc
  regs.set(rd, val);
  pc += 4;
}

/**
 * @brief Executes the srli instruction
 *
 * @param insn The instruction that will be executed
 * @param pos Output stream that will be written to
 ***********************************************************************************************/
void rv32i_hart::exec_srli(uint32_t insn, std::ostream* pos)
{
  //variable definitions to get rd, rs1, and shamt_i
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  int32_t shamt_i = get_rs2(insn);

  //varaible that will be altered to perform shift right logical
  int32_t mask = 0x80000000;

  //set up mask for shift right logical
  shamt_i = shamt_i & 0xfffffe1f;
  mask = ~((mask >> shamt_i) << 1);

  //variable to hold the value stored at rd
  int32_t val = (regs.get(rs1) >> shamt_i) & mask;

  if (pos)
  {
    //output the detailed description of the instruction
    std::string s = render_itype_alu(insn, "srli", shamt_i);
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " >> " <<
	    std::dec << shamt_i << " = " << hex::to_hex0x32(val);
  }

  //set the register to the value and increment the pc
  regs.set(rd, val);
  pc += 4;
}

/**
 * @brief Executes the srai instruction
 *
 * @param insn The instruction that will be executed
 * @param pos Output stream that will be written to
 ***********************************************************************************************/
void rv32i_hart::exec_srai(uint32_t insn, std::ostream* pos)
{
  //variable definitions to get rd, rs1, and shamt_i
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  int32_t shamt_i = get_rs2(insn);

  //variable to hold the value stored at rd
  int32_t val = regs.get(rs1) >> shamt_i;

  if (pos)
  {
    //output the detailed description of the instruction
    std::string s = render_itype_alu(insn, "srai", shamt_i);
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " >> " <<
	    std::dec << shamt_i << " = " << hex::to_hex0x32(val);
  }

  //set the register to the value and increment the pc
  regs.set(rd, val);
  pc += 4;
}

/**
 * @brief Executes the add instruction
 *
 * @param insn The instruction that will be executed
 * @param pos Output stream that will be written to
 ***********************************************************************************************/
void rv32i_hart::exec_add(uint32_t insn, std::ostream* pos)
{
  ///variable definitions to get rd, rs1, and rs2
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  uint32_t rs2 = get_rs2(insn);

  //variable to hold the value stored at rd
  int32_t val = regs.get(rs1) + regs.get(rs2);

  if (pos)
  {
    //output the detailed description of the instruction
    std::string s = render_rtype(insn, "add");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " + " <<
	    hex::to_hex0x32(regs.get(rs2)) << " = " << hex::to_hex0x32(val);
  }

  //set the register to the value and increment the pc
  regs.set(rd, val);
  pc += 4;
}

/**
 * @brief Executes the sub instruction
 *
 * @param insn The instruction that will be executed
 * @param pos Output stream that will be written to
 ***********************************************************************************************/
void rv32i_hart::exec_sub(uint32_t insn, std::ostream* pos)
{
  ///variable definitions to get rd, rs1, and rs2
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  uint32_t rs2 = get_rs2(insn);

  //variable to hold the value stored at rd
  int32_t val = regs.get(rs1) - regs.get(rs2);

  if (pos)
  {
    //output the detailed description of the instruction
    std::string s = render_rtype(insn, "sub");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " - " <<
	    hex::to_hex0x32(regs.get(rs2)) << " = " << hex::to_hex0x32(val);
  }

  //set the register to the value and increment the pc
  regs.set(rd, val);
  pc += 4;
}

/**
 * @brief Executes the sll instruction
 *
 * @param insn The instruction that will be executed
 * @param pos Output stream that will be written to
 ***********************************************************************************************/
void rv32i_hart::exec_sll(uint32_t insn, std::ostream* pos)
{
  //variable definitions to get rd, rs1, and rs2
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  int32_t rs2 = get_rs2(insn);

  //variable to hold the value stored at rd
  int32_t val = regs.get(rs1) << (regs.get(rs2) % XLEN);

  if (pos)
  {
    //output the detailed description of the instruction
    std::string s = render_rtype(insn, "sll");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " << " <<
	    std::dec << (regs.get(rs2) % XLEN) << " = " << hex::to_hex0x32(val);
  }

  //set the register to the value and increment the pc
  regs.set(rd, val);
  pc += 4;
}

/**
 * @brief Executes the slt instruction
 *
 * @param insn The instruction that will be executed
 * @param pos Output stream that will be written to
 *
 * @note Code given in assignment handout
 ***********************************************************************************************/
void rv32i_hart::exec_slt(uint32_t insn, std::ostream* pos)
{
  //variable definition to get rd, rs1, rs2
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  uint32_t rs2 = get_rs2(insn);

  //variable to hold the value stored at rd
  int32_t val = (regs.get(rs1) < regs.get(rs2)) ? 1 : 0;

  if (pos)
  {
    //output the detailed description of the instruction
    std::string s = render_rtype(insn, "slt");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = (" << to_hex0x32(regs.get(rs1)) << " < " <<
	    to_hex0x32(regs.get(rs2)) << ") ? 1 : 0 = " << to_hex0x32(val);
  }

  //set the register to the value and increment the pc
  regs.set(rd, val);
  pc += 4;
}

/**
 * @brief Executes the sltu instruction
 *
 * @param insn The instruction that will be executed
 * @param pos Output stream that will be written to
 ***********************************************************************************************/
void rv32i_hart::exec_sltu(uint32_t insn, std::ostream* pos)
{
  //variable definition to get rd, rs1, rs2
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  uint32_t rs2 = get_rs2(insn);

  //variable to hold the value stored at rd
  uint32_t val = (regs.get(rs1) < regs.get(rs2)) ? 1 : 0;

  if (pos)
  {
    //output the detailed description of the instruction
    std::string s = render_rtype(insn, "sltu");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = (" << to_hex0x32(regs.get(rs1)) << " <U " <<
	    to_hex0x32(regs.get(rs2)) << ") ? 1 : 0 = " << to_hex0x32(val);
  }

  //set the register to the value and increment the pc
  regs.set(rd, val);
  pc += 4;
}

/**
 * @brief Executes the xor instruction
 *
 * @param insn The instruction that will be executed
 * @param pos Output stream that will be written to
 ***********************************************************************************************/
void rv32i_hart::exec_xor(uint32_t insn, std::ostream* pos)
{
  ///variable definitions to get rd, rs1, and rs2
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  uint32_t rs2 = get_rs2(insn);

  //variable to hold the value stored at rd
  int32_t val = regs.get(rs1) ^ regs.get(rs2);

  if (pos)
  {
    //output the detailed description of the instruction
    std::string s = render_rtype(insn, "xor");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " ^ " <<
	    hex::to_hex0x32(regs.get(rs2)) << " = " << hex::to_hex0x32(val);
  }

  //set the register to the value and increment the pc
  regs.set(rd, val);
  pc += 4;
}

/**
 * @brief Executes the srl instruction
 *
 * @param insn The instruction that will be executed
 * @param pos Output stream that will be written to
 ***********************************************************************************************/
void rv32i_hart::exec_srl(uint32_t insn, std::ostream* pos)
{
  //variable definitions to get rd, rs1, and rs2
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  int32_t rs2 = get_rs2(insn);

  //varaible that will be altered to perform shift right logical
  int32_t mask = 0x80000000;
  
  //variables to set up the mask for shift right logical
  int32_t rs2XLEN = regs.get(rs2) % XLEN;
  mask = ~((mask >> rs2XLEN) << 1);

  //variable to hold the value stored a t rd
  int32_t val = (regs.get(rs1) >> rs2XLEN) & mask;

  if (pos)
  {
    //output the detailed description of the instruction
    std::string s = render_rtype(insn, "srl");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " >> " <<
	    std::dec << (regs.get(rs2) % XLEN) << " = " << hex::to_hex0x32(val);
  }

  //set the register to the value and increment the pc
  regs.set(rd, val);
  pc += 4;
}

/**
 * @brief Executes the sra instruction
 *
 * @param insn The instruction that will be executed
 * @param pos Output stream that will be written to
 ***********************************************************************************************/
void rv32i_hart::exec_sra(uint32_t insn, std::ostream* pos)
{
  //variable definitions to get rd, rs1, and rs2
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  int32_t rs2 = get_rs2(insn);

  //variable to hold the value stored at rd
  int32_t val = regs.get(rs1) >> (regs.get(rs2) % XLEN);

  if (pos)
  {
    //output the detailed description of the instruction
    std::string s = render_rtype(insn, "sra");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " >> " <<
	    std::dec << (regs.get(rs2) % XLEN) << " = " << hex::to_hex0x32(val);
  }

  //set the register to the value and increment the pc
  regs.set(rd, val);
  pc += 4;
}

/**
 * @brief Executes the or instruction
 *
 * @param insn The instruction that will be executed
 * @param pos Output stream that will be written to
 ***********************************************************************************************/
void rv32i_hart::exec_or(uint32_t insn, std::ostream* pos)
{
  ///variable definitions to get rd, rs1, and rs2
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  uint32_t rs2 = get_rs2(insn);

  //variable to hold the value stored at rd
  int32_t val = regs.get(rs1) | regs.get(rs2);

  if (pos)
  {
    //output the detailed description of the instruction
    std::string s = render_rtype(insn, "or");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " | " <<
	    hex::to_hex0x32(regs.get(rs2)) << " = " << hex::to_hex0x32(val);
  }

  //set the register to the value and increment the pc
  regs.set(rd, val);
  pc += 4;
}

/**
 * @brief Executes the and instruction
 *
 * @param insn The instruction that will be executed
 * @param pos Output stream that will be written to
 ***********************************************************************************************/
void rv32i_hart::exec_and(uint32_t insn, std::ostream* pos)
{
  ///variable definitions to get rd, rs1, and rs2
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  uint32_t rs2 = get_rs2(insn);

  //variable to hold the value stored at rd
  int32_t val = regs.get(rs1) & regs.get(rs2);

  if (pos)
  {
    //output the detailed description of the instruction
    std::string s = render_rtype(insn, "and");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " & " <<
	    hex::to_hex0x32(regs.get(rs2)) << " = " << hex::to_hex0x32(val);
  }

  //set the register to the value and increment the pc
  regs.set(rd, val);
  pc += 4;
}

/**
 * @brief Executes the ebreak instruction
 *
 * @param insn The instruction that will be executed
 * @param pos Output stream that will be written to
 *
 * @note Code given in assignment handout
 ***********************************************************************************************/
void rv32i_hart::exec_ebreak(uint32_t insn, std::ostream* pos)
{
  if (pos)
  {
    //output that an ebreak has occured
    std::string s = render_ebreak(insn);
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// HALT";
  }

  //set halt to true and halt_reason to ebreak
  halt = true;
  halt_reason = "EBREAK instruction";
}

/**
 * @brief Executes the csrrs instruction
 *
 * @param insn The instruction that will be executed
 * @param pos Output stream that will be written to
 *
 * @note Code given by instructor
 ***********************************************************************************************/
void rv32i_hart::exec_csrrs(uint32_t insn, std::ostream* pos)
{
  //variable definitions to get rd, rs1, and csr
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  int32_t csr = get_imm_i(insn) & 0x00000fff;

  if (csr != 0xf14 || rs1 != 0)
  {
    //halt the instruction
    halt = true;
    halt_reason = "Illegal CSR in CSRSS instruction";
  }

  if (pos)
  {
    //output the detailed description of the instruction
    std::string s = render_csrrx(insn, "csrrs");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << std::dec << mhartid;
  }

  if (!halt)
  {
    //set the register to mhartid and increment the pc
    regs.set(rd, mhartid);
    pc += 4;
  }
}
