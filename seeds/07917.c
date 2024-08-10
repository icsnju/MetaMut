// NOTE: Assertions have been autogenerated by utils/update_cc_test_checks.py
// RUN: %clang_cc1 -triple riscv64 -target-feature +zknd -emit-llvm %s -o - \
// RUN:     -disable-O0-optnone | opt -S -passes=mem2reg \
// RUN:     | FileCheck %s  -check-prefix=RV64ZKND

#include <stdint.h>

// RV64ZKND-LABEL: @aes64dsm(
// RV64ZKND-NEXT:  entry:
// RV64ZKND-NEXT:    [[TMP0:%.*]] = call i64 @llvm.riscv.aes64dsm(i64 [[RS1:%.*]], i64 [[RS2:%.*]])
// RV64ZKND-NEXT:    ret i64 [[TMP0]]
//
uint64_t aes64dsm(uint64_t rs1, uint64_t rs2) {
  return __builtin_riscv_aes64dsm(rs1, rs2);
}


// RV64ZKND-LABEL: @aes64ds(
// RV64ZKND-NEXT:  entry:
// RV64ZKND-NEXT:    [[TMP0:%.*]] = call i64 @llvm.riscv.aes64ds(i64 [[RS1:%.*]], i64 [[RS2:%.*]])
// RV64ZKND-NEXT:    ret i64 [[TMP0]]
//
uint64_t aes64ds(uint64_t rs1, uint64_t rs2) {
  return __builtin_riscv_aes64ds(rs1, rs2);
}


// RV64ZKND-LABEL: @aes64im(
// RV64ZKND-NEXT:  entry:
// RV64ZKND-NEXT:    [[TMP0:%.*]] = call i64 @llvm.riscv.aes64im(i64 [[RS1:%.*]])
// RV64ZKND-NEXT:    ret i64 [[TMP0]]
//
uint64_t aes64im(uint64_t rs1) {
  return __builtin_riscv_aes64im(rs1);
}
