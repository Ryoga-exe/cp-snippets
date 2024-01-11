export type Config = {
  root: string;
  output: string;
  ignore: string[];
  scope: {
    [ext: string]: string;
  };
};

export type SpecComment = {
  [lang: string]: string;
};

export type CommandComment = {
  [lang: string]: string[];
};

export type SnippetItem = {
  prefix: string;
  body: string[];
  description: string;
  scope: string;
  isFileTemplate?: boolean;
};

export type Snippet = {
  [name: string]: SnippetItem;
};
