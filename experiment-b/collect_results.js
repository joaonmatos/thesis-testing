const programs = ["k-means", "matrix-mult", "vec-multiple-returns"];

const inlining = ["inline", "no-inline"];

const optLevels = ["O0", "O2"];

const compilers = ["gcc", "clang", "ccomp"];

const runs = 8;

const textDecoder = new TextDecoder();

function sleep(ms) {
  return new Promise((resolve) => setTimeout(resolve, ms));
}

const out = {};
for (const program of programs) {
  const programOut = {};
  for (const inline of inlining) {
    const inlineOut = {};
    for (const optLevel of optLevels) {
      const compilerOut = {};
      for (const compiler of compilers) {
        console.error(`${program} ${inline} ${compiler} ${optLevel}`);

        const file = `${
          inline === "inline" ? "./woven_code" : "../test-programs"
        }/${program.replace(/-/g, "_")}.c`;
        await Deno.run({
          cmd: [compiler, `-${optLevel}`, file],
          stderr: "inherit",
          stdout: "inherit",
          stdin: "null",
        }).status();

        const runTimes = [];
        for (let i = 0; i < runs; i++) {
          const process = Deno.run({
            cmd: ["./a.out"],
            stderr: "inherit",
            stdout: "piped",
            stdin: "null",
          });
          const output = await process.output();
          runTimes.push(JSON.parse(textDecoder.decode(output)));
          await process.status();
          await sleep(100);
        }

        Deno.removeSync("./a.out");

        const avg = runTimes.reduce((a, b) => a + b, 0) / runTimes.length;
        const stdDev =
          runTimes
            .map((t) => (t - avg) * (t - avg))
            .map((t) => Math.sqrt(t))
            .reduce((a, b) => a + b, 0) / runTimes.length;
        compilerOut[compiler] = { avg, stdDev };

        await sleep(200);
      }
      inlineOut[optLevel] = compilerOut;
    }
    programOut[inline] = inlineOut;
  }
  out[program] = programOut;
}

console.log(JSON.stringify(out, undefined, 2));

console.log();

console.log(
  "program, inlining, optimization level, compiler, average runtime (ms), standard deviation (ms)"
);
for (const p of programs) {
  for (const i of inlining) {
    for (const o of optLevels) {
      for (const c of compilers) {
        console.log(
          `${p}, ${i}, ${o}, ${c}, ${out[p][i][o][c].avg}, ${out[p][i][o][c].stdDev}`
        );
      }
    }
  }
}
