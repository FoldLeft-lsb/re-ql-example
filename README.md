## The reason-react apollo-client trick

The goal here is to make a GraphQL call through apollo-client, without using the JSX Query or Mutation components, doing it in didMount or an event handler. It's not tied to the render function. 

It'll still be a pain to convert the JS objects into Reason records, which I have not done in this demo, but it's not that bad, and you might not need to. 

I'm not certrain what the use-case is yet, I was asked if it were possible to do it, that's how you do it with JS, why can't we do it the same way with Reason?

This lays out a groundwork for how to do queries this way, it can easily be extended, layed out differently. I mean to do mutations as well, but have not got around to it. 

Made with create-react-app with the reason-scripts flag.

Out of habit I use npm to install packages, yarn is fine. ```npm run start``` etc. to run the dev server.

If you want to try it out, feel free to clone this project, or better just copy the code and put it in you own project, there's so little of it. 

#### So

I'm trying to keep this as simple as possible, generic and non-restrictive. 

You might want to make and ignore a config file like I did, for my github Personal Access Token. I do this to get more highly structured data free and easy, and to demonstrate custom headers. 

```
// ./src/config.js

module.exports = {
  github_key: [your key]
};
```

Then, in the .re file,

```
let githubKey = [%bs.raw {|require('../config').github_key|}];
```

Make a client, I've inlined it. 

```
let makeConnection = (~auth_code: string) : ApolloClient.generatedApolloClient =>
  ReasonApollo.createApolloClient(
    ~link=
      ApolloLinks.createHttpLink(
        ~uri="https://api.github.com/graphql",
        ~headers=
          Json.Encode.object_([
            ("Authorization", Js.Json.string("bearer " ++ auth_code)),
            ("Accept", Js.Json.string("application/vnd.github.v3+json")),
          ]),
        (),
      ),
    ~cache=ApolloInMemoryCache.createInMemoryCache(),
    (),
  );
```

You build the query in the same way you do with reason-apollo, then pass your variables into the make call. Query from the github gql api docs. 

```
module ModuleQuery = [%graphql
  {|
    query($number_of_repos:Int!) {
      viewer {
        name
         repositories(last: $number_of_repos) {
           nodes {
             name
           }
         }
       }
    }
|}
];
```

Making the query is a little different, that githubKey being your Personal Access Token for github. I put it here because you might want to determine the logged in user or something. 

So I'm instantiating the client in initialState, could be done anywhere, also doesn't need to be in state. Reql.query is curried, so pass the client in first. The second argument, the query, is typed to 'a to get around the type system, not ideal, something I've failed to lift from reason-apollo, see below. 

I've inlined my make call as well, just beacause. Again that's where I might have variables to work with. 

```
initialState: () => {apolloClient: makeConnection(~auth_code=githubKey)},
didMount: self =>
    Reql.query(
      self.state.apolloClient,
      ModuleQuery.make(~number_of_repos=3, ()) |> Reql.prepareQuery,
    )
    |> Js.Promise.then_(res => {
         ... do stuff
         Js.Promise.resolve();
       })
    |> ignore,
```

That Reql.prepareQuery function just runs the query through the parser. It turns a string into a ReasonApolloTypes.queryString.

```
/* Reql.re */
[@bs.module] external gql : ReasonApolloTypes.gql = "graphql-tag";

[@bs.send] external query : (ApolloClient.generatedApolloClient, 'a) => Js.Promise.t('b) = "query";

let prepareQuery = q => {
  "query": gql(. q##query),
  "parse": q##parse,
  "variables": q##variables,
};
```

You may notice I haven't typed my requests at all, the types were very restrictive and I can get away without them. Reason-apollo does type them I think, but I'm subverting quite a lot of it. 

It is dangerous, but the compilation target is dangerous, I would be ok with that in JS, just be responsible... 