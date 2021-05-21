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

#ifndef RV32I_HART_H
#define RV32I_HART_H

#include <iostream>
#include <string>
#include <cassert>
#include "rv32i_decode.h"
#include "hex.h"
#include "memory.h"
#include "registerfile.h"

/**
 * Subclass of rv32i_decode to represent the execution unit of a RV32I hart
 **********************************************************************************************/
class rv32i_hart : public rv32i_decode
{
public:
  /**
   * @breif Constructor to initialize mem as a reference
   *
   * @note Code given in assignment handout
   **********************************************************************************************/
  rv32i_hart(memory &m) : mem(m) { }

  /**
   * @breif Mutator for show_instructions
   * 
   * @param b If true, show each instruction that is executed with a comment
   *
   * @note Code given in assignment handout
   **********************************************************************************************/
  void set_show_instructions(bool b) { show_instructions = b; }

  /**
   * @breif Mutator for show_registers
   * 
   * @param b If true, dump the registers before instruction is executed
   *
   * @note Code given in assignment handout
   **********************************************************************************************/
  void set_show_registers(bool b) { show_registers = b; }

  /**
   * @breif Accessor for halt
   *
   * @return halt
   *
   * @note Code given in assignment handout
   **********************************************************************************************/
  bool is_halted() const { return halt; };

  /**
   * @breif Return a string indicating the reason the hart has been halted
   * 
   * @return halt_reasons
   *
   * @note Code given in assignment handout
   **********************************************************************************************/
  const std::string &get_halt_reason() const { return halt_reason; }

  /**
   * @breif Accessor for insn_counter
   * 
   * @return insn_counter
   *
   * @note Code given in assignment handout
   **********************************************************************************************/
  uint64_t get_insn_counter() const { return insn_counter; }

  /**
   * @breif Mutator fo mhartid
   * 
   * @param Set the ID value to be returned by the csrrs instruction for CSR register number 0xf14
   *
   * @note Code given in assignment handout
   **********************************************************************************************/
  void set_mhartid(int i) { mhartid = i; }

  void tick(const std::string &hdr="");
  void dump(const std::string &hdr="") const;
  void reset();

private:
  static constexpr int instruction_width = 35;	//The amount of space to give each instruction
  void exec(uint32_t insn, std::ostream*);
  void exec_illegal_insn(uint32_t insn, std::ostream*);
  void exec_lui(uint32_t insn, std::ostream* pos);
  void exec_auipc(uint32_t insn, std::ostream* pos);
  void exec_jal(uint32_t insn, std::ostream* pos);
  void exec_jalr(uint32_t insn, std::ostream* pos);
  void exec_beq(uint32_t insn, std::ostream* pos);
  void exec_bne(uint32_t insn, std::ostream* pos);
  void exec_blt(uint32_t insn, std::ostream* pos);
  void exec_bge(uint32_t insn, std::ostream* pos);
  void exec_bltu(uint32_t insn, std::ostream* pos);
  void exec_bgeu(uint32_t insn, std::ostream* pos);
  void exec_lb(uint32_t insn, std::ostream* pos);
  void exec_lh(uint32_t insn, std::ostream* pos);
  void exec_lw(uint32_t insn, std::ostream* pos);
  void exec_lbu(uint32_t insn, std::ostream* pos);
  void exec_lhu(uint32_t insn, std::ostream* pos);
  void exec_sb(uint32_t insn, std::ostream* pos);
  void exec_sh(uint32_t insn, std::ostream* pos);
  void exec_sw(uint32_t insn, std::ostream* pos);
  void exec_addi(uint32_t insn, std::ostream* pos);
  void exec_slti(uint32_t insn, std::ostream* pos);
  void exec_sltiu(uint32_t insn, std::ostream* pos);
  void exec_xori(uint32_t insn, std::ostream* pos);
  void exec_ori(uint32_t insn, std::ostream* pos);
  void exec_andi(uint32_t insn, std::ostream* pos);
  void exec_slli(uint32_t insn, std::ostream* pos);
  void exec_srli(uint32_t insn, std::ostream* pos);
  void exec_srai(uint32_t insn, std::ostream* pos);
  void exec_add(uint32_t insn, std::ostream* pos);
  void exec_sub(uint32_t insn, std::ostream* pos);
  void exec_sll(uint32_t insn, std::ostream* pos);
  void exec_slt(uint32_t insn, std::ostream* pos);
  void exec_sltu(uint32_t insn, std::ostream* pos);
  void exec_xor(uint32_t insn, std::ostream* pos);
  void exec_srl(uint32_t insn, std::ostream* pos);
  void exec_sra(uint32_t insn, std::ostream* pos);
  void exec_or(uint32_t insn, std::ostream* pos);
  void exec_and(uint32_t insn, std::ostream* pos);
  void exec_ecall(uint32_t insn, std::ostream* pos);
  void exec_ebreak(uint32_t insn, std::ostream* pos);
  void exec_csrrs(uint32_t insn, std::ostream* pos);

  bool halt = { false }; 		//Flag to stop the hart from executing instructions
  std::string halt_reason = { "none" }; //If halt is true, set this to contain a string describing the reason for the halt
  uint32_t mhartid = { 0 }; 		//The CSR register value to return by a csrrs instruction that reads register 0xf14
  bool show_instructions = { false }; 	//When true, print each instruction when simulating its execution
  bool show_registers = { false }; 	//When true, print a dump of the hart state before executing each instruction
  uint64_t insn_counter = { 0 }; 	//Counts the number of instructions that have been executed
  uint32_t pc = { 0 }; 			//Contains the address of the instruction being decoded/disassembled

protected:
  registerfile regs; 			//The GP-regs for the simulation
  memory &mem; 				//Fetches instructions and reads/writes data in the load and store instructions
};

#endif
