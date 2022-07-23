laraImport("lara.benchmark.NasBenchmarkSet");
laraImport("clava.autopar.Parallelize");
laraImport("clava.autopar.AutoParStats");
laraImport("clava.opt.NormalizeToSubset");
laraImport("weaver.Query");
laraImport("clava.Clava");

const set = new NasBenchmarkSet();
// set.setInputSizes("W", "A", "B");
// set.setBenchmarks("BT", "CG", "FT", "LU", "MG", "SP");
set.setInputSizes("B");
set.setBenchmarks("SP");

// println("Baseline");

// for (const instance of set) {
//   instance.compile();
//   instance.execute();
// }

// println("AutoPar");

for (const instance of set) {
  try {
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

// println("Normalize + AutoPar");

// for (const instance of set) {
//   try {
//     NormalizeToSubset();
//     Parallelize.forLoops();
//     println(JSON.stringify(AutoParStats.get(), null, 2));
//     AutoParStats.reset();
//     instance.compile();
//     instance.execute();
//   } catch (e) {
//     println(e);
//   }
// }

// println("Normalize");

// for (const instance of set) {
//   try {
//     NormalizeToSubset();
//     Parallelize.forLoops();
//     println(JSON.stringify(AutoParStats.get(), null, 2));
//     AutoParStats.reset();
//     instance.compile();
//     instance.execute();
//   } catch (e) {
//     println(e);
//   }
// }

// println("Normalize + Inline");

// for (const instance of set) {
//   try {
//     NormalizeToSubset();
//     const calls_old = Query.search("call").get().length;
//     Inlining();
//     const calls_new = Query.search("call").get().length;
//     println("calls", calls_old, calls_new);
//     instance.compile();
//     instance.execute();
//   } catch (e) {
//     println(e);
//   }
// }
