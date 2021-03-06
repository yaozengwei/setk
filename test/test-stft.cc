// test-stft.cc
// wujian@18.2.12

#include "include/stft.h"

using namespace kaldi;

void TestStft() {
  bool binary;
  Input wave_in("orig.wav", &binary);
  WaveData wave_orig;
  wave_orig.Read(wave_in.Stream());

  // configs
  ShortTimeFTOptions opts;
  opts.frame_length = 1024;
  opts.center = true;
  opts.normalize_input = false;

  // BaseFloat range = wave_orig.Data().LargestAbsElem();
  ShortTimeFTComputer stft_computer(opts);

  Matrix<BaseFloat> specs;
  stft_computer.ShortTimeFT(wave_orig.Data(), &specs);

  Matrix<BaseFloat> recon;
  stft_computer.InverseShortTimeFT(specs, &recon, -1);

  Output ko("copy.wav", binary, false);
  WaveData wave_copy(16000, recon);
  wave_copy.Write(ko.Stream());
  // std:: cout << vec << std::endl;
}

void TestRealfft() {
  int32 dim = 16;

  Vector<BaseFloat> vec(dim);
  vec.SetRandn();
  std::cout << vec;
  RealFft(&vec, true);
  std::cout << vec;
  RealFft(&vec, false);
  vec.Scale(1.0 / dim);
  std::cout << vec;
}

void TestIstft() {
  bool binary;
  Input wave_in("orig.wav", &binary);
  WaveData wave_orig;
  wave_orig.Read(wave_in.Stream());

  // configs
  ShortTimeFTOptions opts;
  opts.frame_length = 1024;
  opts.frame_shift = 256;
  opts.normalize_input = false;
  opts.apply_log = true;
  opts.apply_pow = true;

  ShortTimeFTComputer stft_computer(opts);

  Matrix<BaseFloat> stft_orig, specs, arg;
  stft_computer.Compute(wave_orig.Data(), &stft_orig, &specs, &arg);
  BaseFloat range = wave_orig.Data().LargestAbsElem();
  /*
  stft_computer.ShortTimeFT(wave_orig.Data(), &stft_orig);
  stft_computer.ComputeSpectrum(stft_orig, &specs);
  stft_computer.ComputeArg(stft_orig, &arg);
  */

  Matrix<BaseFloat> stft_recon;
  stft_computer.Polar(specs, arg, &stft_recon);

  std::cout << stft_orig.Row(10);
  std::cout << stft_recon.Row(10);

  Matrix<BaseFloat> recon;
  stft_computer.InverseShortTimeFT(stft_recon, &recon, range);

  Output ko("copy.wav", binary, false);
  WaveData wave_copy(16000, recon);
  wave_copy.Write(ko.Stream());
  // std:: cout << vec << std::endl;
}

int main() {
  TestStft();
  TestIstft();
  return 0;
}
