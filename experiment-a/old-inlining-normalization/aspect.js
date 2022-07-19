laraImport("clava.code.Inliner");
laraImport("weaver.Query");
laraImport("clava.opt.NormalizeToSubset");
laraImport("clava.opt.PrepareForInlining");

let calls_total = 0;
let calls_no_definition = 0;
let calls_inlined = 0;
let calls_inlining_fails = 0;

NormalizeToSubset();

for (const call of Query.search("call")) {
  calls_total += 1;
  if (!call.function.isImplementation) {
    calls_no_definition += 1;
    continue;
  }
  try {
    println(
      `Trying to inline call '${call.code}' at location ${call.location}`
    );
    PrepareForInlining(call.function);
    call.inline();
    println("Inlined successfully\n");
    calls_inlined += 1;
  } catch (e) {
    println(`Inlining failed: ${e.message}\n`);
    calls_inlining_fails += 1;
  }
}

println("Results:");
println(
  JSON.stringify(
    { calls_total, calls_no_definition, calls_inlined, calls_inlining_fails },
    undefined,
    2
  )
);
