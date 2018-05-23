type module_query_t = {
  .
  "parse": Js.Json.t => {. "hello": option(string)},
  "query": ReasonApolloTypes.queryString,
  "variables": Js.Json.t,
};