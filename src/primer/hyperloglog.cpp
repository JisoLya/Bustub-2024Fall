#include "primer/hyperloglog.h"
#include <algorithm>
#include <bitset>
#include <cmath>

namespace bustub {

template <typename KeyType>
HyperLogLog<KeyType>::HyperLogLog(int16_t n_bits) : cardinality_(0) {
  std::cout << "HyperLogLog<KeyType>::HyperLogLog() initial bits = " << n_bits << std::endl;
  if (n_bits < 0) return;
  this->mbuckets_ = std::vector<int>(1 << n_bits, 0);
  this->n_bits_ = n_bits;
}

template <typename KeyType>
auto HyperLogLog<KeyType>::ComputeBinary(const hash_t &hash) const -> std::bitset<BITSET_CAPACITY> {
  /** @TODO(student) Implement this function! */
  std::cout << "HyperLogLog<KeyType>::ComputeBinary input " << hash << std::endl;
  return {hash};
}

/**
 *
 * @param bset
 * @return 除去标志位第一个1比特的位置，索引由1开始
 */
template <typename KeyType>
auto HyperLogLog<KeyType>::PositionOfLeftmostOne(const std::bitset<BITSET_CAPACITY> &bset) const -> uint64_t {
  /** @TODO(student) Implement this function! */
  std::cout << "HyperLogLog<KeyType>::PositionOfLeftmostOne input " << bset << std::endl;
  for (int i = BITSET_CAPACITY - 1 - n_bits_; i >= 0; i--) {
    // 找到第一个为1的比特
    if (bset[i]) {
      std::cout << "Position of leftmost one = " << BITSET_CAPACITY - i - n_bits_ << std::endl;
      return BITSET_CAPACITY - i - n_bits_;
    }
  }
  return 0;
}

template <typename KeyType>
auto HyperLogLog<KeyType>::AddElem(KeyType val) -> void {
  /** @TODO(student) Implement this function! */
  // 计算hash
  auto hash = CalculateHash(val);
  std::cout << "HyperLogLog<KeyType>::AddElem element : " << val << " hash = " << hash << std::endl;

  // 获取bucket
  std::bitset<BITSET_CAPACITY> bset = ComputeBinary(hash);
  const uint64_t posOfLeftMostOne = PositionOfLeftmostOne(bset);
  std::cout << "HyperLogLog<KeyType>::AddElem pos = " << posOfLeftMostOne << std::endl;

  // 获取bucket中的索引
  const std::string idxStr = bset.to_string().substr(0, n_bits_);
  // 将bucket设置为max(p, bucket[j])
  size_t idx = 0;
  std::cout << "idx str = " << idxStr << std::endl;
  if (!idxStr.empty()) {
    idx = std::stoi(idxStr, nullptr, 2);
    std::cout << "parse idx = " << idx << std::endl;
  }
  const auto newdata = std::max<int>(mbuckets_[idx], posOfLeftMostOne);
  std::cout << "bucket[" << idx << "]is set to:" << newdata << std::endl;
  this->mbuckets_[idx] = newdata;
}
template <typename KeyType>
auto HyperLogLog<KeyType>::ComputeCardinality() -> void {
  /** @TODO(student) Implement this function! */
  const int m = this->mbuckets_.capacity();
  double sigma = 0;
  for (auto &bucket : this->mbuckets_) {
    sigma += std::pow(2, -bucket);
  }
  this->cardinality_ = CONSTANT * m * m / sigma;
}

template class HyperLogLog<int64_t>;
template class HyperLogLog<std::string>;

}  // namespace bustub
