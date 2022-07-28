laraImport("lara.benchmark.NasBenchmarkSet");
laraImport("clava.autopar.Parallelize");
laraImport("clava.autopar.AutoParStats");
laraImport("clava.opt.NormalizeToSubset");
laraImport("weaver.Query");
laraImport("clava.Clava");

const set = new NasBenchmarkSet();
set.setInputSizes("W", "A", "B");
set.setBenchmarks("BT", "CG", "FT", "LU", "MG", "SP");

println("Baseline");

for (const instance of set) {
  instance.compile();
  instance.execute();
}

println("AutoPar");

for (const instance of set) {
  try {
    Parallelize.forLoops();
    println(JSON.stringify(AutoParStats.get(), null, 2));
    Parallelize.removeNestedPragmas();
    AutoParStats.reset();
    instance.getCMaker().addFlags("-fopenmp");
    instance.compile();
    // instance.execute();
  } catch (e) {
    println(e);
  }
}

println("Normalize + AutoPar");

for (const instance of set) {
  try {
    NormalizeToSubset();
    Parallelize.forLoops();
    println(JSON.stringify(AutoParStats.get(), null, 2));
    Parallelize.removeNestedPragmas();
    AutoParStats.reset();
    instance.getCMaker().addFlags("-fopenmp");
    instance.compile();
    instance.execute();
  } catch (e) {
    println(e);
  }
}
