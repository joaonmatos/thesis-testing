laraImport("weaver.Query");
laraImport("clava.code.Inliner");
laraImport("clava.code.StatementDecomposer");
laraImport("clava.opt.NormalizeToSubset");

const main = Query.search("function", "main").get()[0];

NormalizeToSubset();

new Inliner(new StatementDecomposer()).inlineFunctionTree(main);
