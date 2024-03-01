function buildtoolbox(toolboxFolder, outDir, version)
    versionNumber = regexp(version, "v(\d+\.\d+\.\d+)", "tokens");
    versionNumber = versionNumber{1};

    releaseName = strjoin(['igraph-SE2', version], '_');

    uuid = "d6b3abc6-7541-4fb2-b636-fc353c928a87";
    opts = matlab.addons.toolbox.ToolboxOptions(toolboxFolder, uuid);

    opts.ToolboxName = "igraph-SE2";
    opts.ToolboxVersion = versionNumber;
    opts.ToolboxGettingStartedGuide = fullfile(toolboxFolder, ...
                                               "GettingStarted.mlx");

    opts.AuthorName = "David R. Connell";
    opts.Summary = "SpeakEasy2 community detection algorithm.";
    opts.Description = ...
        "A rewrite of the SpeakEasy2 community detection algorithm in C " + ...
        "for improved performance.";

    if ~exist(outDir, "dir")
        mkdir(outDir);
    end
    opts.OutputFile = fullfile(outDir, releaseName);

    opts.MinimumMatlabRelease = "R2019b";
    opts.MaximumMatlabRelease = "";

    matlab.addons.toolbox.packageToolbox(opts);
end
