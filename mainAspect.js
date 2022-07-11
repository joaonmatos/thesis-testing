laraImport("lara.benchmark.NasBenchmarkSet");
laraImport("lara.code.Timer");
laraImport("weaver.Query");
laraImport("clava.opt.Inlining");

const nasSet = new NasBenchmarkSet();
nasSet.setBenchmarks("EP");

for (const benchmark of nasSet) {
  Inlining();
  // const timer = new Timer();

  println(Query.root().code);
  //benchmark

  benchmark.getCMaker().addFlags("-O0");

  benchmark.execute();
}
